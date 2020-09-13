precision mediump float;

//uniform 	vec4 		u_color;
uniform 	sampler2D 	u_texture0;
uniform		float		u_fix_alpha;
varying 	vec2 		v_uv;

void main()
{
	gl_FragColor = texture2D(u_texture0,  v_uv) - vec4(0, 0, 0, u_fix_alpha);
}
