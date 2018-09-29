#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// アルファマスク シェーダー カラーフィルタ適用 (VertBuf TexcBuf)

static char *localGlobalVertSrc =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat_pos;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

static char *localGlobalFragSrc =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform float fs_unif_fil_col_alp;"
	"uniform mat4 fs_unif_fil_col_mat;"
	"uniform vec4 fs_unif_fil_col_vec;"
	"uniform sampler2D texture;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;"
		"vec4 filterColor = fragColor * fs_unif_fil_col_mat + fs_unif_fil_col_vec;"
		"gl_FragColor = mix(fragColor, filterColor, fs_unif_fil_col_alp);"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineGraphicEngineShader shader;
	GLint program;
	GLint unif_fil_col_alp;
	GLint unif_fil_col_mat;
	GLint unif_fil_col_vec;
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
	localGlobal.shader.attr_uvc = glGetAttribLocation(localGlobal.program, "vs_attr_uvc");
	localGlobal.shader.unif_mat_pos = glGetUniformLocation(localGlobal.program, "vs_unif_mat_pos");
	localGlobal.shader.unif_col = glGetUniformLocation(localGlobal.program, "fs_unif_col");
	localGlobal.unif_fil_col_alp = glGetUniformLocation(localGlobal.program, "fs_unif_fil_col_alp");
	localGlobal.unif_fil_col_mat = glGetUniformLocation(localGlobal.program, "fs_unif_fil_col_mat");
	localGlobal.unif_fil_col_vec = glGetUniformLocation(localGlobal.program, "fs_unif_fil_col_vec");
}

// 使用開始
static void use(void){
	glEnableVertexAttribArray(localGlobal.shader.attr_pos);
	glEnableVertexAttribArray(localGlobal.shader.attr_uvc);
}

// 使用完了
static void unuse(void){
	glDisableVertexAttribArray(localGlobal.shader.attr_pos);
	glDisableVertexAttribArray(localGlobal.shader.attr_uvc);
}

// 破棄
static void dispose(void){
	glDeleteProgram(localGlobal.program);
}

// アルファマスク シェーダー カラーフィルタ適用 使用準備
void engineGraphicShaderTextureFilterColorUse(void){
	if(engineGraphicEngineShaderIsUse(&localGlobal.shader)){return;}
	localGlobal.shader.setup = setup;
	localGlobal.shader.unuse = unuse;
	localGlobal.shader.dispose = dispose;
	engineGraphicEngineShaderUse(&localGlobal.shader);
	glUseProgram(localGlobal.program);
	use();

	// 深度バッファ設定 2次元通常描画
	engineGraphicEngineSetDepthMask(false);
	engineGraphicEngineSetDepthTest(false);
	glDisable(GL_CULL_FACE);

	// 半透明アルファ合成
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
}

// ----------------------------------------------------------------

// アルファマスク シェーダー カラーフィルタ適用 カラーフィルタの設定
void engineGraphicShaderTextureFilterColorSetFilterColorAlpha(double a){
	if(!engineGraphicEngineShaderIsUse(&localGlobal.shader)){return;}
	glUniform1f(localGlobal.unif_fil_col_alp, a);
}

// アルファマスク シェーダー カラーフィルタ適用 カラーフィルタの設定
void engineGraphicShaderTextureFilterColorSetFilterColorMatrix(struct engineMathMatrix44 *matrix){
	if(!engineGraphicEngineShaderIsUse(&localGlobal.shader)){return;}
	glUniformMatrix4fv(localGlobal.unif_fil_col_mat, 1, GL_FALSE, matrix->m);
}

// アルファマスク シェーダー カラーフィルタ適用 カラーフィルタの設定
void engineGraphicShaderTextureFilterColorSetFilterColorVector(struct engineMathVector4 *vector){
	if(!engineGraphicEngineShaderIsUse(&localGlobal.shader)){return;}
	glUniform4fv(localGlobal.unif_fil_col_vec, 1, vector->v);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

