
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// カラーブレンド アルファマスク シェーダー (VertBuf Clor3Buf TexcBuf)

char *externGlobal_shader_textureColorBlendAlphaMask_vert_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_col;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat_pos;"
	"varying vec4 color;"
	"varying vec2 texCoord;"
	"void main(){"
		"color = vec4(vs_attr_col, 1.0);"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_shader_textureColorBlendAlphaMask_frag_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec4 color;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col * color;"
		"if(fragColor.a < 0.8){discard;}"
		"gl_FragColor = fragColor;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

