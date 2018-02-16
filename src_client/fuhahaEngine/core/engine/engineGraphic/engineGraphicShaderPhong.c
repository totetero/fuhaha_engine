
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// フォンシェーダー (VertBuf NormBuf TexcBuf)

char *externGlobal_shader_phong_vert_src =
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
		"vs_normal = vs_unif_mat_nrm * vs_attr_nrm;"
		"gl_Position = vs_unif_mat_pos * vec4(vs_attr_pos, 1.0);"
	"}";

char *externGlobal_shader_phong_frag_src =
	"precision highp float;"
	"uniform vec4 fs_unif_col;"
	"uniform sampler2D texture;"
	"varying vec3 vs_normal;"
	"varying vec2 texCoord;"
	"void main(){"
		"vec4 fragColor = texture2D(texture, texCoord) * fs_unif_col;"
		"vec3 lightPosition = vec3(-1.0, 1.0, -3.0);"
		"vec3 lightAmbient = vec3(0.2, 0.2, 0.2);"
		"vec3 lightDiffuse = vec3(0.6, 0.6, 0.6);"
		"vec3 lightSpecular = vec3(0.5, 0.5, 0.5);"
		"float shininess = 10.0;"
		"vec3 light = normalize(lightPosition);"
		"vec3 normal = normalize(vs_normal);"
		"float diff = max(dot(light, normal), 0.0);"
		"vec3 refl = reflect(-light, normal);"
		"vec3 view = -vec3(0.0, 0.0, 1.0);"
		"float spec = pow(max(dot(refl, view), 0.0), shininess);"
		"vec4 lightColor = vec4(diff * lightDiffuse + spec * lightSpecular + lightAmbient, 1.0);"
		"gl_FragColor = lightColor * fragColor;"
	"}";

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

