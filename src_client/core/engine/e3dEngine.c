#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 重複動作阻止のための状態記録
static enum e3dModeDraw e3dMemoryModeDraw;
static enum e3dModeStencil e3dMemoryModeStencil;
static GLboolean e3dMemoryModeDepth;
static enum e3dTexType e3dMemoryTexType;
static GLuint e3dMemoryTexData;
static GLfloat e3dMemoryColor[4];
static e3dObjectVBOId e3dMemoryVertVBO;
static e3dObjectVBOId e3dMemoryClorVBO;
static e3dObjectVBOId e3dMemoryTexcVBO;
static e3dObjectIBOId e3dMemoryFaceIBO;

// e3dシェーダー構造体
struct e3dShader{
	GLint program;
	GLint attr_pos;
	GLint attr_col;
	GLint attr_uvc;
	GLint unif_mat;
	GLint unif_col;
};

// e3dシェーダー
static struct e3dShader *e3dCurrentShader;
static struct e3dShader e3dReserveShader1;
static struct e3dShader e3dReserveShader2;
static struct e3dShader e3dReserveShader3;
static struct e3dShader e3dReserveShader4;

// ----------------------------------------------------------------

// シェーダープログラム作成関数
static void e3dShaderCreate(struct e3dShader *shader, char *vssrc, char *fsstr){
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
	shader->attr_col = glGetAttribLocation(shader->program, "vs_attr_col");
	shader->attr_uvc = glGetAttribLocation(shader->program, "vs_attr_uvc");
	shader->unif_mat = glGetUniformLocation(shader->program, "vs_unif_mat");
	shader->unif_col = glGetUniformLocation(shader->program, "fs_unif_col");
}

// 初期化
void e3dEngineInit(){
	char *vsh1_src = "precision highp float;attribute vec3 vs_attr_pos;attribute vec2 vs_attr_uvc;uniform mat4 vs_unif_mat;varying vec2 texCoord;void main(){texCoord = vs_attr_uvc;gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);}";
	char *fsh1_src = "precision highp float;uniform vec4 fs_unif_col;uniform sampler2D texture;varying vec2 texCoord;void main(){vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;if(fragColor.a > 0.8){gl_FragColor = fragColor;}else{discard;}}";
	char *vsh2_src = "precision highp float;attribute vec3 vs_attr_pos;attribute vec2 vs_attr_uvc;uniform mat4 vs_unif_mat;varying vec2 texCoord;void main(){texCoord = vs_attr_uvc;gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);}";
	char *fsh2_src = "precision highp float;uniform vec4 fs_unif_col;uniform sampler2D texture;varying vec2 texCoord;void main(){vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;gl_FragColor = fragColor;}";
	char *vsh3_src = "precision highp float;attribute vec3 vs_attr_pos;attribute vec3 vs_attr_col;attribute vec2 vs_attr_uvc;uniform mat4 vs_unif_mat;varying vec4 color;varying vec2 texCoord;void main(){color = vec4(vs_attr_col, 1.0);texCoord = vs_attr_uvc;gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);}";
	char *fsh3_src = "precision highp float;uniform vec4 fs_unif_col;uniform sampler2D texture;varying vec4 color;varying vec2 texCoord;void main(){vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col * color;if(fragColor.a > 0.8){gl_FragColor = fragColor;}else{discard;}}";
	char *vsh4_src = "precision highp float;attribute vec3 vs_attr_pos;attribute vec3 vs_attr_col;uniform mat4 vs_unif_mat;varying vec4 color;void main(){color = vec4(vs_attr_col, 1.0);gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);}";
	char *fsh4_src = "precision highp float;uniform vec4 fs_unif_col;varying vec4 color;void main(){gl_FragColor = fs_unif_col * color;}";
	e3dShaderCreate(&e3dReserveShader1, vsh1_src, fsh1_src);
	e3dShaderCreate(&e3dReserveShader2, vsh2_src, fsh2_src);
	e3dShaderCreate(&e3dReserveShader3, vsh3_src, fsh3_src);
	e3dShaderCreate(&e3dReserveShader4, vsh4_src, fsh4_src);

	e3dMemoryModeDraw = -1;
	e3dMemoryModeStencil = -1;
	e3dMemoryModeDepth = GL_TRUE;
	e3dMemoryColor[0] = -1;
	e3dMemoryResetVBO();
	e3dMemoryResetIBO();
	e3dMemoryResetTex();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glClearStencil(0);
	glCullFace(GL_BACK);
	//glEnable(GL_TEXTURE_2D); // コメントを外すとブラウザでなんか警告が出る
	glEnable(GL_BLEND);
}

// ----------------------------------------------------------------

// e3d命令 描画のクリア
void e3dClearAll(){
	glDepthMask(e3dMemoryModeDepth = GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	e3dSetStencilMode(E3DMODESTENCIL_NONE);
}

// e3d命令 深度バッファのクリア
void e3dClearDepth(){
	glClear(GL_DEPTH_BUFFER_BIT);
}

// e3d命令 ステンシルバッファのクリア
void e3dClearStencil(){
	glClear(GL_STENCIL_BUFFER_BIT);
}

// 重複動作阻止のためのVBO状態記録をリセット
void e3dMemoryResetVBO(){
	e3dMemoryVertVBO = 0;
	e3dMemoryClorVBO = 0;
	e3dMemoryTexcVBO = 0;
}

// 重複動作阻止のためのIBO状態記録をリセット
void e3dMemoryResetIBO(){
	e3dMemoryFaceIBO = 0;
}

// 重複動作阻止のためのTex状態記録をリセット
void e3dMemoryResetTex(){
	e3dMemoryTexType = -1;
	e3dMemoryTexData = ~0;
}

// ----------------------------------------------------------------

// e3d命令 描画モード設定
void e3dSetDrawMode(enum e3dModeDraw mode){
	if(e3dMemoryModeDraw == mode){return;}
	e3dMemoryModeDraw = mode;

	if(e3dCurrentShader != NULL){
		if(e3dCurrentShader->attr_pos >= 0){glDisableVertexAttribArray(e3dCurrentShader->attr_pos);}
		if(e3dCurrentShader->attr_col >= 0){glDisableVertexAttribArray(e3dCurrentShader->attr_col);}
		if(e3dCurrentShader->attr_uvc >= 0){glDisableVertexAttribArray(e3dCurrentShader->attr_uvc);}
	}

	switch(mode){
		case E3DMODEDRAW_NORMAL:
			// 汎用モード (VertBuf TexcBuf)
			e3dCurrentShader = &e3dReserveShader1;
			glDepthMask(e3dMemoryModeDepth = GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
			break;
		case E3DMODEDRAW_2D:
			// 2D描画モード (VertBuf TexcBuf)
			e3dCurrentShader = &e3dReserveShader2;
			glDepthMask(e3dMemoryModeDepth = GL_FALSE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE); // 半透明アルファ合成
			break;
		case E3DMODEDRAW_ALPHA_ADD:
			// アルファ合成モード (VertBuf TexcBuf)
			e3dCurrentShader = &e3dReserveShader2;
			glDepthMask(e3dMemoryModeDepth = GL_FALSE);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE); // 加算合成
			break;
		case E3DMODEDRAW_HKNW:
			// ハコニワ地形モード (VertBuf Clor3Buf TexcBuf)
			e3dCurrentShader = &e3dReserveShader3;
			glDepthMask(e3dMemoryModeDepth = GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
			break;
		case E3DMODEDRAW_SPHERE:
			// スフィア地形モード (VertBuf Clor3Buf)
			e3dCurrentShader = &e3dReserveShader4;
			glDepthMask(e3dMemoryModeDepth = GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE); // 半透明アルファ合成
			break;
	}

	glUseProgram(e3dCurrentShader->program);
	if(e3dCurrentShader->attr_pos >= 0){glEnableVertexAttribArray(e3dCurrentShader->attr_pos);}
	if(e3dCurrentShader->attr_col >= 0){glEnableVertexAttribArray(e3dCurrentShader->attr_col);}
	if(e3dCurrentShader->attr_uvc >= 0){glEnableVertexAttribArray(e3dCurrentShader->attr_uvc);}

	e3dMemoryColor[0] = -1;
	e3dMemoryResetVBO();
}

// e3d命令 ステンシルマスクモード設定
void e3dSetStencilMode(enum e3dModeStencil mode){
	if(e3dMemoryModeStencil == mode){return;}
	e3dMemoryModeStencil = mode;

	// ステンシル有効設定
	if(mode != E3DMODESTENCIL_NONE){glEnable(GL_STENCIL_TEST);}else{glDisable(GL_STENCIL_TEST);}

	// ステンシル以外の描画制限設定
	switch(mode){
		case E3DMODESTENCIL_MASK_0:
		case E3DMODESTENCIL_MASK_1:
		case E3DMODESTENCIL_MASK_2:
		case E3DMODESTENCIL_READ_EQ1_MASK_0:
		case E3DMODESTENCIL_READ_EQ1_MASK_2:
		case E3DMODESTENCIL_READ_GE1_MASK_0:
		case E3DMODESTENCIL_READ_GE1_MASK_INCR:
		case E3DMODESTENCIL_READ_LE1_MASK_0:
		case E3DMODESTENCIL_READ_LE1_MASK_INCR:
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			break;
		default:
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(e3dMemoryModeDepth);
			break;
	}

	// ステンシル条件設定
	switch(mode){
		case E3DMODESTENCIL_NONE: break;
		case E3DMODESTENCIL_MASK_0:              glStencilFunc(GL_ALWAYS, 0, ~0); break;
		case E3DMODESTENCIL_MASK_1:              glStencilFunc(GL_ALWAYS, 1, ~0); break;
		case E3DMODESTENCIL_MASK_2:              glStencilFunc(GL_ALWAYS, 2, ~0); break;
		case E3DMODESTENCIL_WRITE_0:             glStencilFunc(GL_ALWAYS, 0, ~0); break;
		case E3DMODESTENCIL_WRITE_1:             glStencilFunc(GL_ALWAYS, 1, ~0); break;
		case E3DMODESTENCIL_WRITE_2:             glStencilFunc(GL_ALWAYS, 2, ~0); break;
		case E3DMODESTENCIL_READ_EQ0:            glStencilFunc(GL_EQUAL,  0, ~0); break;
		case E3DMODESTENCIL_READ_EQ1:            glStencilFunc(GL_EQUAL,  1, ~0); break;
		case E3DMODESTENCIL_READ_EQ2:            glStencilFunc(GL_EQUAL,  2, ~0); break;
		case E3DMODESTENCIL_READ_EQ1_MASK_0:     glStencilFunc(GL_EQUAL,  1, ~0); break;
		case E3DMODESTENCIL_READ_EQ1_WRITE_0:    glStencilFunc(GL_EQUAL,  1, ~0); break;
		case E3DMODESTENCIL_READ_EQ1_MASK_2:     glStencilFunc(GL_EQUAL,  1, ~0); break;
		case E3DMODESTENCIL_READ_EQ1_WRITE_2:    glStencilFunc(GL_EQUAL,  1, ~0); break;
		case E3DMODESTENCIL_READ_GE1:            glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_GE1_MASK_0:     glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_GE1_WRITE_0:    glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_GE1_MASK_INCR:  glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_GE1_WRITE_INCR: glStencilFunc(GL_LEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_LE1:            glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_LE1_MASK_0:     glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_LE1_WRITE_0:    glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_LE1_MASK_INCR:  glStencilFunc(GL_GEQUAL, 1, ~0); break;
		case E3DMODESTENCIL_READ_LE1_WRITE_INCR: glStencilFunc(GL_GEQUAL, 1, ~0); break;
	}

	// ステンシル条件設定
	switch(mode){
		case E3DMODESTENCIL_MASK_0:
		case E3DMODESTENCIL_MASK_1:
		case E3DMODESTENCIL_MASK_2:
		case E3DMODESTENCIL_WRITE_0:
		case E3DMODESTENCIL_WRITE_1:
		case E3DMODESTENCIL_WRITE_2:
			// マスクの書き込み
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			break;
		case E3DMODESTENCIL_READ_EQ0:
		case E3DMODESTENCIL_READ_EQ1:
		case E3DMODESTENCIL_READ_EQ2:
		case E3DMODESTENCIL_READ_GE1:
		case E3DMODESTENCIL_READ_LE1:
			// マスク使用
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			break;
		case E3DMODESTENCIL_READ_EQ1_MASK_0:
		case E3DMODESTENCIL_READ_EQ1_WRITE_0:
		case E3DMODESTENCIL_READ_GE1_MASK_0:
		case E3DMODESTENCIL_READ_GE1_WRITE_0:
		case E3DMODESTENCIL_READ_LE1_MASK_0:
		case E3DMODESTENCIL_READ_LE1_WRITE_0:
			// マスク書き換え 0にする
			glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
			break;
		case E3DMODESTENCIL_READ_EQ1_MASK_2:
		case E3DMODESTENCIL_READ_EQ1_WRITE_2:
		case E3DMODESTENCIL_READ_GE1_MASK_INCR:
		case E3DMODESTENCIL_READ_GE1_WRITE_INCR:
		case E3DMODESTENCIL_READ_LE1_MASK_INCR:
		case E3DMODESTENCIL_READ_LE1_WRITE_INCR:
			// マスク書き換え 1増加
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			break;
		default:
			break;
	}
}

// e3d命令 深度バッファを一時的に無効化
void e3dIgnoreDepthMode(bool isIgnore){
	if(!e3dMemoryModeDepth){return;}
	if(isIgnore){
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}else{
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
}

// ----------------------------------------------------------------

// e3d命令 テクスチャを指定
void e3dBindTexture(e3dObjectTexId e3dId){
	GLuint glId;
	enum e3dTexType type;
	if(!e3dObjectTexGetGLId(e3dId, &glId, &type)){return;}
	if(e3dMemoryTexType == type && e3dMemoryTexData == glId){return;}

	if(e3dMemoryTexData != glId){glBindTexture(GL_TEXTURE_2D, glId);}

	switch(type){
		case E3DTEXTYPE_LINEAR:
			// 線形補完 汎用
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case E3DTEXTYPE_NEAREST:
			// 最近傍法 ドット絵地形用
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
	}

	e3dMemoryTexType = type;
	e3dMemoryTexData = glId;
}

// e3d命令 VBO登録 頂点座標
void e3dBindVertVBO(e3dObjectVBOId e3dId){
	if(e3dMemoryVertVBO == e3dId){return;}
	e3dMemoryVertVBO = e3dId;

	GLuint glId;
	if(!e3dObjectVBOGetGLId(e3dId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(e3dCurrentShader->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// e3d命令 VBO登録 カラーrgb
void e3dBindClorVBO(e3dObjectVBOId e3dId){
	if(e3dMemoryClorVBO == e3dId){return;}
	e3dMemoryClorVBO = e3dId;

	GLuint glId;
	if(!e3dObjectVBOGetGLId(e3dId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(e3dCurrentShader->attr_col, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// e3d命令 VBO登録 テクスチャ座標
void e3dBindTexcVBO(e3dObjectVBOId e3dId){
	if(e3dMemoryTexcVBO == e3dId){return;}
	e3dMemoryTexcVBO = e3dId;

	GLuint glId;
	if(!e3dObjectVBOGetGLId(e3dId, &glId)){return;}
	glBindBuffer(GL_ARRAY_BUFFER, glId);
	glVertexAttribPointer(e3dCurrentShader->attr_uvc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

// e3d命令 IBO登録 頂点インデックス
void e3dBindFaceIBO(e3dObjectIBOId e3dId){
	if(e3dMemoryFaceIBO == e3dId){return;}
	e3dMemoryFaceIBO = e3dId;

	GLuint glId;
	if(!e3dObjectIBOGetGLId(e3dId, &glId)){return;}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glId);
}

// ----------------------------------------------------------------

// e3d命令 行列の設定
void e3dSetMatrix(struct e3dMatrix44 *matrix){
	GLfloat fmatrix[16] = {
		(GLfloat)matrix->m00, (GLfloat)matrix->m01, (GLfloat)matrix->m02, (GLfloat)matrix->m03,
		(GLfloat)matrix->m10, (GLfloat)matrix->m11, (GLfloat)matrix->m12, (GLfloat)matrix->m13,
		(GLfloat)matrix->m20, (GLfloat)matrix->m21, (GLfloat)matrix->m22, (GLfloat)matrix->m23,
		(GLfloat)matrix->m30, (GLfloat)matrix->m31, (GLfloat)matrix->m32, (GLfloat)matrix->m33,
	};
	glUniformMatrix4fv(e3dCurrentShader->unif_mat, 1, GL_FALSE, fmatrix);
}

// e3d命令 色の設定
void e3dSetColor(double r, double g, double b, double a){
	GLfloat fcolor[4] = {(GLfloat)r, (GLfloat)g, (GLfloat)b, (GLfloat)a};
	if(memcmp(e3dMemoryColor, fcolor, 4 * sizeof(GLfloat))){
		memcpy(e3dMemoryColor, fcolor, 4 * sizeof(GLfloat));
		glUniform4fv(e3dCurrentShader->unif_col, 1, fcolor);
	}
}

// ----------------------------------------------------------------

// e3d命令 頂点インデックスを元に描画
void e3dDrawIndex(uint32_t offset, uint32_t count){
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (GLushort*)NULL + offset);
}

// e3d命令クラス 描画確定
void e3dFlush(){
	glFlush();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

