#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// 重複動作阻止のための状態記録
	struct{
		struct engineGraphicEngineShader *shader;
		enum engineGraphicTextureType texType;
		engineGraphicObjectVBOId vertVBO;
		engineGraphicObjectVBOId normVBO;
		engineGraphicObjectVBOId clorVBO;
		engineGraphicObjectVBOId texcVBO;
		engineGraphicObjectIBOId faceIBO;
		struct engineMathVector4 color;
		bool isDepthMask;
		bool isDepthTest;
	} memory;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
void engineGraphicEngineInit(void){
	engineGraphicEngineMemoryResetVBO();
	engineGraphicEngineMemoryResetIBO();
	engineGraphicEngineMemoryResetTex();
	engineGraphicEngineMemoryResetColor();
	localGlobal.memory.isDepthMask = true;
	localGlobal.memory.isDepthTest = true;

	glClearColor(DEFINESETTING_BACKGROUNDCOLOR);
	glClearDepthf(1.0f);
	glCullFace(GL_BACK);
	//glEnable(GL_TEXTURE_2D); // コメントを外すとブラウザでなんか警告が出る
	glEnable(GL_BLEND);
}

// 解放
void engineGraphicEngineExit(void){
	// TODO
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ビューポート設定
void engineGraphicEngineViewport(int x, int y, int w, int h){
	double pixelRatio = global.window.pixelRatio;
	GLint vx = (GLint)(x * pixelRatio);
	GLint vy = (GLint)((global.window.h - y - h) * pixelRatio);
	GLsizei vw = (GLsizei)(w * pixelRatio);
	GLsizei vh = (GLsizei)(h * pixelRatio);
	glViewport(vx, vy, vw, vh);
}

// ----------------------------------------------------------------

// シェーダー使用を宣言する
void engineGraphicEngineShaderUse(struct engineGraphicEngineShader *shader){
	// 直前に使っていたシェーダーの使用完了設定
	if(localGlobal.memory.shader != NULL && localGlobal.memory.shader->isUse){
		localGlobal.memory.shader->unuse();
		localGlobal.memory.shader->isUse = false;
	}

	// 準備前確認
	bool isSetup = (shader->prev != NULL && shader->next != NULL);
	if(!isSetup){
		if(localGlobal.memory.shader == NULL){
			// 最初の要素を設定
			shader->prev = shader;
			shader->next = shader;
		}else{
			// 双方向循環リストの要素を設定
			struct engineGraphicEngineShader *shaderPrev = localGlobal.memory.shader->prev;
			struct engineGraphicEngineShader *shaderNext = localGlobal.memory.shader;
			shader->prev = shaderPrev;
			shader->next = shaderNext;
			shaderPrev->next = shader;
			shaderNext->prev = shader;
		}
		// 準備
		shader->setup();
	}

	// 使用設定 シェーダーをリストの先頭に持ってゆきフラグを立てる
	localGlobal.memory.shader = shader;
	localGlobal.memory.shader->isUse = true;
	// 重複動作阻止のメモリーリセット
	engineGraphicEngineMemoryResetVBO();
	engineGraphicEngineMemoryResetColor();
}

// シェーダー使用中確認
bool engineGraphicEngineShaderIsUse(struct engineGraphicEngineShader *shader){
	return (localGlobal.memory.shader == shader && localGlobal.memory.shader->isUse);
}

// シェーダー破棄
void engineGraphicEngineShaderDispose(struct engineGraphicEngineShader *shader){
	//// 使用中のシェーダーは破棄できない TODO ダメだろ
	//if(engineGraphicEngineShaderIsUse(shader)){return;}
	
	// 同じシェーダーがリストの先頭にある場合は先頭を変える
	if(localGlobal.memory.shader == shader){localGlobal.memory.shader = shader->next;}
	if(localGlobal.memory.shader == shader){localGlobal.memory.shader = NULL;}

	// 双方向循環リストから外す
	struct engineGraphicEngineShader *shaderPrev = shader->prev;
	struct engineGraphicEngineShader *shaderNext = shader->next;
	shaderPrev->next = shaderPrev;
	shaderNext->prev = shaderNext;
	shader->prev = NULL;
	shader->next = NULL;
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 描画のクリア
void engineGraphicEngineClearAll(void){
	if(!localGlobal.memory.isDepthMask){glDepthMask(GL_TRUE);}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!localGlobal.memory.isDepthMask){glDepthMask(GL_FALSE);}
}

// グラフィックエンジン命令 深度バッファのクリア
void engineGraphicEngineClearDepth(void){
	glClear(GL_DEPTH_BUFFER_BIT);
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 描画モード設定
void engineGraphicEngineSetDrawMode(enum engineGraphicEngineModeDraw mode){
	switch(mode){
		case ENGINEGRAPHICENGINEMODEDRAW_3D:                           engineGraphicShaderTextureAlphaMaskUse(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_NORMAL:              engineGraphicShaderTextureUse3dAlphaNormal(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_ADD:                 engineGraphicShaderTextureUse3dAlphaAdd(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL:              engineGraphicShaderTextureUse2dAlphaNormal(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_ADD:                 engineGraphicShaderTextureUse2dAlphaAdd(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_FILTER_COLOR_ALPHA_NORMAL: engineGraphicShaderTextureFilterColorUse(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_PHONG:                        engineGraphicShaderPhoneUse(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_HKNW:                         engineGraphicShaderTextureColorBlendAlphaMaskUse(); break;
		case ENGINEGRAPHICENGINEMODEDRAW_SPHERE:                       engineGraphicShaderColorBlendrUse(); break;
	}
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 深度バッファを設定
void engineGraphicEngineSetDepthMask(bool isDepthMask){
	localGlobal.memory.isDepthMask = isDepthMask;
	if(localGlobal.memory.isDepthMask){
		glDepthMask(GL_TRUE);
	}else{
		glDepthMask(GL_FALSE);
	}
}

// グラフィックエンジン命令 深度バッファを設定
void engineGraphicEngineSetDepthTest(bool isDepthTest){
	localGlobal.memory.isDepthTest = isDepthTest;
	if(localGlobal.memory.isDepthTest){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}
}

// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthMask(bool isIgnore){
	if(!localGlobal.memory.isDepthMask){return;}
	if(isIgnore){
		glDepthMask(GL_FALSE);
	}else{
		glDepthMask(GL_TRUE);
	}
}

// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthTest(bool isIgnore){
	if(!localGlobal.memory.isDepthTest){return;}
	if(isIgnore){
		glDisable(GL_DEPTH_TEST);
	}else{
		glEnable(GL_DEPTH_TEST);
	}
}

// ----------------------------------------------------------------

// 重複動作阻止のためのVBO状態記録をリセット
void engineGraphicEngineMemoryResetVBO(void){
	localGlobal.memory.vertVBO = 0;
	localGlobal.memory.normVBO = 0;
	localGlobal.memory.clorVBO = 0;
	localGlobal.memory.texcVBO = 0;
}

// 重複動作阻止のためのIBO状態記録をリセット
void engineGraphicEngineMemoryResetIBO(void){
	localGlobal.memory.faceIBO = 0;
}

// 重複動作阻止のためのTex状態記録をリセット
void engineGraphicEngineMemoryResetTex(void){
	localGlobal.memory.texType = -1;
}

// 重複動作阻止のための色をリセット
void engineGraphicEngineMemoryResetColor(void){
	engineMathVec4Set(&localGlobal.memory.color, 0, 0, 0, -1);
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 テクスチャを指定
void engineGraphicEngineBindTexture(engineGraphicTextureId egtId){
	GLuint glId;
	enum engineGraphicTextureType type;
	if(!engineGraphicTextureGetGLId(egtId, &glId, &type)){return;}
	engineGraphicEngineBindTextureGlId(glId, type);
}

// グラフィックエンジン命令 テクスチャを指定
void engineGraphicEngineBindTextureGlId(GLuint glId, enum engineGraphicTextureType type){
	bool isBind = corePluginTextureIsBind(glId);
	if(localGlobal.memory.texType == type && isBind){return;}

	if(!isBind){glBindTexture(GL_TEXTURE_2D, glId);}

	switch(type){
		case ENGINEGRAPHICTEXTURETYPE_LINEAR:
			// 線形補完 汎用
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case ENGINEGRAPHICTEXTURETYPE_NEAREST:
			// 最近傍法 ドット絵地形用
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
	}

	localGlobal.memory.texType = type;
}

// グラフィックエンジン命令 VBO登録 頂点座標
void engineGraphicEngineBindVertVBO(engineGraphicObjectVBOId egoId){
	if(localGlobal.memory.vertVBO == egoId){return;}
	localGlobal.memory.vertVBO = egoId;

	GLuint glId;
	if(!engineGraphicObjectVBOGetGLId(egoId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(localGlobal.memory.shader->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// グラフィックエンジン命令 VBO登録 法線方向
void engineGraphicEngineBindNormVBO(engineGraphicObjectVBOId egoId){
	if(localGlobal.memory.normVBO == egoId){return;}
	localGlobal.memory.normVBO = egoId;

	GLuint glId;
	if(!engineGraphicObjectVBOGetGLId(egoId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(localGlobal.memory.shader->attr_nrm, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// グラフィックエンジン命令 VBO登録 カラーrgb
void engineGraphicEngineBindClorVBO(engineGraphicObjectVBOId egoId){
	if(localGlobal.memory.clorVBO == egoId){return;}
	localGlobal.memory.clorVBO = egoId;

	GLuint glId;
	if(!engineGraphicObjectVBOGetGLId(egoId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(localGlobal.memory.shader->attr_col, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// グラフィックエンジン命令 VBO登録 テクスチャ座標
void engineGraphicEngineBindTexcVBO(engineGraphicObjectVBOId egoId){
	if(localGlobal.memory.texcVBO == egoId){return;}
	localGlobal.memory.texcVBO = egoId;

	GLuint glId;
	if(!engineGraphicObjectVBOGetGLId(egoId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(localGlobal.memory.shader->attr_uvc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

// グラフィックエンジン命令 IBO登録 頂点インデックス
void engineGraphicEngineBindFaceIBO(engineGraphicObjectIBOId egoId){
	if(localGlobal.memory.faceIBO == egoId){return;}
	localGlobal.memory.faceIBO = egoId;

	GLuint glId;
	if(!engineGraphicObjectIBOGetGLId(egoId, &glId)){return;}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glId);
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 行列の設定
void engineGraphicEngineSetMatrix(struct engineMathMatrix44 *matrix){
	glUniformMatrix4fv(localGlobal.memory.shader->unif_mat_pos, 1, GL_FALSE, matrix->m);
}

// グラフィックエンジン命令 行列の設定
void engineGraphicEngineSetMatrixNorm(struct engineMathMatrix44 *matrixProjection, struct engineMathMatrix44 *matrixModelView){
	// モデルビュー行列と投影行列を掛け合わせた行列
	struct engineMathMatrix44 matrixModelViewProjection;
	engineMathMat4Multiply(&matrixModelViewProjection, matrixProjection, matrixModelView);
	glUniformMatrix4fv(localGlobal.memory.shader->unif_mat_pos, 1, GL_FALSE, matrixModelViewProjection.m);
	// 逆転置行列
	struct engineMathMatrix33 matrixNormal;
	engineMathMat3Copy4(&matrixNormal, matrixModelView);
	engineMathMat3Invert(&matrixNormal);
	engineMathMat3Transpose(&matrixNormal);
	glUniformMatrix3fv(localGlobal.memory.shader->unif_mat_nrm, 1, GL_FALSE, matrixNormal.m);
}

// グラフィックエンジン命令 色の設定
void engineGraphicEngineSetColorRgba(double r, double g, double b, double a){
	struct engineMathVector4 color;
	engineMathVec4Set(&color, r, g, b, a);
	engineGraphicEngineSetColorVec(&color);
}
void engineGraphicEngineSetColorVec(struct engineMathVector4 *color){
	if(memcmp(&localGlobal.memory.color, color, sizeof(struct engineMathVector4))){
		memcpy(&localGlobal.memory.color, color, sizeof(struct engineMathVector4));
		glUniform4fv(localGlobal.memory.shader->unif_col, 1, color->v);
	}
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 頂点インデックスを元に描画
void engineGraphicEngineDrawIndex(int offset, int count){
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (GLushort*)NULL + offset);
}

// グラフィックエンジン命令クラス 描画確定
void engineGraphicEngineFlush(void){
	glFlush();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

