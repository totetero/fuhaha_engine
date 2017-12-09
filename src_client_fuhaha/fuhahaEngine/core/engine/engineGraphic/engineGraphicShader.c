
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// アルファマスク シェーダー (VertBuf TexcBuf)

char *externGlobal_vsh1_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_fsh1_src =
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
// アルファブレンド シェーダー (VertBuf TexcBuf)

char *externGlobal_vsh2_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat;"
	"varying vec2 texCoord;"
	"void main(){"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_fsh2_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;"
		"gl_FragColor = fragColor;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// カラーブレンド アルファマスク シェーダー (VertBuf Clor3Buf TexcBuf)

char *externGlobal_vsh3_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_col;"
	"attribute vec2 vs_attr_uvc;"
	"uniform mat4 vs_unif_mat;"
	"varying vec4 color;"
	"varying vec2 texCoord;"
	"void main(){"
		"color = vec4(vs_attr_col, 1.0);"
		"texCoord = vs_attr_uvc;"
		"gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_fsh3_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec4 color;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col * color;"
		"if(fragColor.a > 0.8){"
			"gl_FragColor = fragColor;"
		"}else{"
			"discard;"
		"}"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// カラーブレンド テクスチャ無し シェーダー (VertBuf Clor3Buf)

char *externGlobal_vsh4_src =
	"precision highp float;"
	"attribute vec3 vs_attr_pos;"
	"attribute vec3 vs_attr_col;"
	"uniform mat4 vs_unif_mat;"
	"varying vec4 color;"
	"void main(){"
		"color = vec4(vs_attr_col, 1.0);"
		"gl_Position = vs_unif_mat * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_fsh4_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"varying vec4 color;"
	"void main(){"
		"gl_FragColor = fs_unif_col * color;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

