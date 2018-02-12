
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// アルファマスク シェーダー (VertBuf TexcBuf)

char *externGlobal_shader_textureAlphaMask_vert_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_shader_textureAlphaMask_frag_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;"
		"if(fragColor.a > 0.8){"
			"gl_FragColor = fragColor;"
		"}else{"
			"discard;"
		"}"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

