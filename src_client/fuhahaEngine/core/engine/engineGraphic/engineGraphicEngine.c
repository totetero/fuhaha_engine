#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineGraphic/engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グラフィックエンジンシェーダー構造体
struct engineGraphicEngineShader{
	GLint program;
	GLint attr_pos;
	GLint attr_nrm;
	GLint attr_col;
	GLint attr_uvc;
	GLint unif_mat;
	GLint unif_col;
};

static struct{
	// グラフィックエンジンシェーダー
	struct{
		struct engineGraphicEngineShader texture;
		struct engineGraphicEngineShader textureAlphaMask;
		struct engineGraphicEngineShader textureColorBlendAlphaMask;
		struct engineGraphicEngineShader colorBlend;
	} shader;
	// 重複動作阻止のための状態記録
	struct{
		struct engineGraphicEngineShader *shader;
		enum engineGraphicEngineModeDraw modeDraw;
		bool modeDepthMask;
		bool modeDepthTest;
		enum engineGraphicTextureType texType;
		struct engineMathVector4 color;
		engineGraphicObjectVBOId vertVBO;
		engineGraphicObjectVBOId normVBO;
		engineGraphicObjectVBOId clorVBO;
		engineGraphicObjectVBOId texcVBO;
		engineGraphicObjectIBOId faceIBO;
	} memory;
} localGlobal = {0};

// ----------------------------------------------------------------

// シェーダープログラム作成関数
static void engineGraphicEngineShaderCreate(struct engineGraphicEngineShader *shader, char *vssrc, char *fsstr){
	shader->program = glCreateProgram();
	GLint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vshader, 1, (const GLchar**)&vssrc, NULL);
	glShaderSource(fshader, 1, (const GLchar**)&fsstr, NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);
	glAttachShader(shader->program, vshader);
	glAttachShader(shader->program, fshader);
	glLinkProgram(shader->program);
	shader->attr_pos = glGetAttribLocation(shader->program, "vs_attr_pos");
	shader->attr_nrm = glGetAttribLocation(shader->program, "vs_attr_nrm");
	shader->attr_col = glGetAttribLocation(shader->program, "vs_attr_col");
	shader->attr_uvc = glGetAttribLocation(shader->program, "vs_attr_uvc");
	shader->unif_mat = glGetUniformLocation(shader->program, "vs_unif_mat");
	shader->unif_col = glGetUniformLocation(shader->program, "fs_unif_col");
}

// 初期化
void engineGraphicEngineInit(void){
	engineGraphicEngineShaderCreate(&localGlobal.shader.texture, externGlobal_shader_texture_vert_src, externGlobal_shader_texture_frag_src);
	engineGraphicEngineShaderCreate(&localGlobal.shader.textureAlphaMask, externGlobal_shader_textureAlphaMask_vert_src, externGlobal_shader_textureAlphaMask_frag_src);
	engineGraphicEngineShaderCreate(&localGlobal.shader.textureColorBlendAlphaMask, externGlobal_shader_textureColorBlendAlphaMask_vert_src, externGlobal_shader_textureColorBlendAlphaMask_frag_src);
	engineGraphicEngineShaderCreate(&localGlobal.shader.colorBlend, externGlobal_shader_colorBlend_vert_src, externGlobal_shader_colorBlend_frag_src);

	localGlobal.memory.shader = NULL;
	localGlobal.memory.modeDraw = -1;
	localGlobal.memory.modeDepthMask = true;
	localGlobal.memory.modeDepthTest = true;
	engineMathVec4Set(&localGlobal.memory.color, 0, 0, 0, -1);
	engineGraphicEngineMemoryResetVBO();
	engineGraphicEngineMemoryResetIBO();
	engineGraphicEngineMemoryResetTex();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glCullFace(GL_BACK);
	//glEnable(GL_TEXTURE_2D); // コメントを外すとブラウザでなんか警告が出る
	glEnable(GL_BLEND);
}

// 解放
void engineGraphicEngineExit(void){
	glDeleteProgram(localGlobal.shader.texture.program);
	glDeleteProgram(localGlobal.shader.textureAlphaMask.program);
	glDeleteProgram(localGlobal.shader.textureColorBlendAlphaMask.program);
	glDeleteProgram(localGlobal.shader.colorBlend.program);
}

// ----------------------------------------------------------------

// グラフィックエンジン命令 描画のクリア
void engineGraphicEngineClearAll(void){
	if(!localGlobal.memory.modeDepthMask){glDepthMask(GL_TRUE);}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!localGlobal.memory.modeDepthMask){glDepthMask(GL_FALSE);}
}

// グラフィックエンジン命令 深度バッファのクリア
void engineGraphicEngineClearDepth(void){
	glClear(GL_DEPTH_BUFFER_BIT);
}

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

// ----------------------------------------------------------------

// グラフィックエンジン命令 描画モード設定
void engineGraphicEngineSetDrawMode(enum engineGraphicEngineModeDraw mode){
	if(localGlobal.memory.modeDraw == mode){return;}
	localGlobal.memory.modeDraw = mode;

	// シェーダー差し替え
	struct engineGraphicEngineShader *oldShader = localGlobal.memory.shader;
	switch(mode){
		case ENGINEGRAPHICENGINEMODEDRAW_3D:              localGlobal.memory.shader = &localGlobal.shader.textureAlphaMask; break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_NORMAL: localGlobal.memory.shader = &localGlobal.shader.texture; break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_ADD:    localGlobal.memory.shader = &localGlobal.shader.texture; break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL: localGlobal.memory.shader = &localGlobal.shader.texture; break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_ADD:    localGlobal.memory.shader = &localGlobal.shader.texture; break;
		case ENGINEGRAPHICENGINEMODEDRAW_HKNW:            localGlobal.memory.shader = &localGlobal.shader.textureColorBlendAlphaMask; break;
		case ENGINEGRAPHICENGINEMODEDRAW_SPHERE:          localGlobal.memory.shader = &localGlobal.shader.colorBlend; break;
	}
	if(localGlobal.memory.shader != oldShader){
		if(oldShader != NULL && oldShader->attr_pos >= 0){glDisableVertexAttribArray(oldShader->attr_pos);}
		if(oldShader != NULL && oldShader->attr_nrm >= 0){glDisableVertexAttribArray(oldShader->attr_nrm);}
		if(oldShader != NULL && oldShader->attr_col >= 0){glDisableVertexAttribArray(oldShader->attr_col);}
		if(oldShader != NULL && oldShader->attr_uvc >= 0){glDisableVertexAttribArray(oldShader->attr_uvc);}
		glUseProgram(localGlobal.memory.shader->program);
		if(localGlobal.memory.shader->attr_pos >= 0){glEnableVertexAttribArray(localGlobal.memory.shader->attr_pos);}
		if(localGlobal.memory.shader->attr_nrm >= 0){glEnableVertexAttribArray(localGlobal.memory.shader->attr_nrm);}
		if(localGlobal.memory.shader->attr_col >= 0){glEnableVertexAttribArray(localGlobal.memory.shader->attr_col);}
		if(localGlobal.memory.shader->attr_uvc >= 0){glEnableVertexAttribArray(localGlobal.memory.shader->attr_uvc);}
		engineMathVec4Set(&localGlobal.memory.color, 0, 0, 0, -1);
		engineGraphicEngineMemoryResetVBO();
	}

	// oprnGL設定
	switch(mode){
		case ENGINEGRAPHICENGINEMODEDRAW_3D:
			// 3D描画モード (VertBuf TexcBuf)
			localGlobal.memory.modeDepthMask = true;
			localGlobal.memory.modeDepthTest = true;
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE); // アルファ無視のアルファマスク
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_NORMAL:
			// 3D描画アルファ合成モード (VertBuf TexcBuf)
			localGlobal.memory.modeDepthMask = false;
			localGlobal.memory.modeDepthTest = true;
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE); // 半透明アルファ合成
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_ADD:
			// 3D描画アルファ合成モード (VertBuf TexcBuf)
			localGlobal.memory.modeDepthMask = false;
			localGlobal.memory.modeDepthTest = true;
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE); // 加算合成
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL:
			// 2D描画アルファ合成モード (VertBuf TexcBuf)
			localGlobal.memory.modeDepthMask = false;
			localGlobal.memory.modeDepthTest = false;
			glDisable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE); // 半透明アルファ合成
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_ADD:
			// 2D描画アルファ合成モード (VertBuf TexcBuf)
			localGlobal.memory.modeDepthMask = false;
			localGlobal.memory.modeDepthTest = false;
			glDisable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE); // 加算合成
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_HKNW:
			// ハコニワ地形モード (VertBuf Clor3Buf TexcBuf)
			localGlobal.memory.modeDepthMask = true;
			localGlobal.memory.modeDepthTest = true;
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE); // アルファ無視のアルファマスク
			break;
		case ENGINEGRAPHICENGINEMODEDRAW_SPHERE:
			// スフィア地形モード (VertBuf Clor3Buf)
			localGlobal.memory.modeDepthMask = true;
			localGlobal.memory.modeDepthTest = true;
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE); // 半透明アルファ合成
			break;
	}
	// 深度設定
	if(localGlobal.memory.modeDepthMask){glDepthMask(GL_TRUE);}else{glDepthMask(GL_FALSE);}
	if(localGlobal.memory.modeDepthTest){glEnable(GL_DEPTH_TEST);}else{glDisable(GL_DEPTH_TEST);}
}

// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthMask(bool isIgnore){
	if(!localGlobal.memory.modeDepthMask){return;}
	if(isIgnore){
		glDepthMask(GL_FALSE);
	}else{
		glDepthMask(GL_TRUE);
	}
}

// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthTest(bool isIgnore){
	if(!localGlobal.memory.modeDepthTest){return;}
	if(isIgnore){
		glDisable(GL_DEPTH_TEST);
	}else{
		glEnable(GL_DEPTH_TEST);
	}
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
	glUniformMatrix4fv(localGlobal.memory.shader->unif_mat, 1, GL_FALSE, matrix->m);
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

