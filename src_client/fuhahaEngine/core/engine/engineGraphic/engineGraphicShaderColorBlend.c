#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// カラーブレンド テクスチャ無し シェーダー (VertBuf Clor3Buf)

char *externGlobal_shader_colorBlend_vert_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_col;"
	"uniform mat4 vs_unif_mat_pos;"
	"varying vec4 color;"
	"void main(){"
		"color = vec4(vs_attr_col, 1.0);"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_shader_colorBlend_frag_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"varying vec4 color;"
	"void main(){"
		"gl_FragColor = fs_unif_col * color;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

