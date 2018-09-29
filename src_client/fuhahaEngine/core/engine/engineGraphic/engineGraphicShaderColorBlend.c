#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// カラーブレンド テクスチャ無し シェーダー (VertBuf Clor3Buf)

static char *localGlobalVertSrc =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_col;"
	"uniform mat4 vs_unif_mat_pos;"
	"varying vec4 color;"
	"void main(){"
		"color = vec4(vs_attr_col, 1.0);"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

static char *localGlobalFragSrc =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"varying vec4 color;"
	"void main(){"
		"gl_FragColor = fs_unif_col * color;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineGraphicEngineShader shader;
	GLint program;
} localGlobal = {0};

// ----------------------------------------------------------------

// シェーダープログラム作成関数
static GLint programCreate(char *vssrc, char *fsstr){
	GLint program = glCreateProgram();
	GLint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vshader, 1, (const GLchar**)&vssrc, NULL);
	glShaderSource(fshader, 1, (const GLchar**)&fsstr, NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	return program;
}

// 設定
static void setup(void){
	localGlobal.program = programCreate(localGlobalVertSrc, localGlobalFragSrc);
	localGlobal.shader.attr_pos = glGetAttribLocation(localGlobal.program, "vs_attr_pos");
	localGlobal.shader.attr_col = glGetAttribLocation(localGlobal.program, "vs_attr_col");
	localGlobal.shader.unif_mat_pos = glGetUniformLocation(localGlobal.program, "vs_unif_mat_pos");
	localGlobal.shader.unif_col = glGetUniformLocation(localGlobal.program, "fs_unif_col");
}

// 使用開始
static void use(void){
	glEnableVertexAttribArray(localGlobal.shader.attr_pos);
	glEnableVertexAttribArray(localGlobal.shader.attr_col);
}

// 使用完了
static void unuse(void){
	glDisableVertexAttribArray(localGlobal.shader.attr_pos);
	glDisableVertexAttribArray(localGlobal.shader.attr_col);
}

// 破棄
static void dispose(void){
	glDeleteProgram(localGlobal.program);
}

// カラーブレンド テクスチャ無し シェーダー 使用準備
void engineGraphicShaderColorBlendrUse(void){
	if(engineGraphicEngineShaderIsUse(&localGlobal.shader)){return;}
	localGlobal.shader.setup = setup;
	localGlobal.shader.unuse = unuse;
	localGlobal.shader.dispose = dispose;
	engineGraphicEngineShaderUse(&localGlobal.shader);
	glUseProgram(localGlobal.program);
	use();

	// 深度バッファ設定 3次元通常描画
	engineGraphicEngineSetDepthMask(true);
	engineGraphicEngineSetDepthTest(true);
	glEnable(GL_CULL_FACE);

	// 半透明アルファ合成
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

