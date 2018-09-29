#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// フォン シェーダー (VertBuf NormBuf TexcBuf)

static char *localGlobalVertSrc =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_nrm;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat_pos;"
	"uniform mat3 vs_unif_mat_nrm;"
	"varying vec3 vs_normal;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"vs_normal = normalize(vs_unif_mat_nrm * vs_attr_nrm);"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

static char *localGlobalFragSrc =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec3 vs_normal;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 texColor = texture2D(texture, texCoord) * fs_unif_col;"
		"vec3 view = vec3(0.0, 0.0, 1.0);"
		"vec3 lightPosition = vec3(-1.0, 1.0, 1.0);"
		"vec3 lightAmbient = vec3(0.2, 0.2, 0.2);"
		"vec3 lightDiffuse = vec3(0.6, 0.6, 0.6);"
		"vec3 lightSpecular = vec3(0.5, 0.5, 0.5);"
		"float shininess = 50.0;"
		"vec3 light = normalize(lightPosition);"
		"vec3 normal = normalize(vs_normal);"
		"float diff = max(dot(light, normal), 0.0);"
		"vec3 refl = reflect(-light, normal);"
		"float spec = pow(max(dot(refl, view), 0.0), shininess);"
		"vec4 lightColor = vec4(diff * lightDiffuse + spec * lightSpecular + lightAmbient, 1.0);"
		"gl_FragColor = lightColor * texColor;"
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
	localGlobal.shader.attr_nrm = glGetAttribLocation(localGlobal.program, "vs_attr_nrm");
	localGlobal.shader.attr_uvc = glGetAttribLocation(localGlobal.program, "vs_attr_uvc");
	localGlobal.shader.unif_mat_pos = glGetUniformLocation(localGlobal.program, "vs_unif_mat_pos");
	localGlobal.shader.unif_mat_nrm = glGetUniformLocation(localGlobal.program, "vs_unif_mat_nrm");
	localGlobal.shader.unif_col = glGetUniformLocation(localGlobal.program, "fs_unif_col");
}

// 使用開始
static void use(void){
	glEnableVertexAttribArray(localGlobal.shader.attr_pos);
	glEnableVertexAttribArray(localGlobal.shader.attr_nrm);
	glEnableVertexAttribArray(localGlobal.shader.attr_uvc);
}

// 使用完了
static void unuse(void){
	glDisableVertexAttribArray(localGlobal.shader.attr_pos);
	glDisableVertexAttribArray(localGlobal.shader.attr_nrm);
	glDisableVertexAttribArray(localGlobal.shader.attr_uvc);
}

// 破棄
static void dispose(void){
	glDeleteProgram(localGlobal.program);
}

// フォン シェーダー 使用準備
void engineGraphicShaderPhoneUse(void){
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

	// アルファブレンド無視
	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

