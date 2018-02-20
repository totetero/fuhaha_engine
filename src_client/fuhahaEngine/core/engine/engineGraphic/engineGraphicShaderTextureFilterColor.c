
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// アルファマスク シェーダー カラーフィルタ適用 (VertBuf TexcBuf)

char *externGlobal_shader_textureFilterColor_vert_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat_pos;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_shader_textureFilterColor_frag_src =
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

