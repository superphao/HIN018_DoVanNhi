attribute 	vec3 	a_posL;
attribute 	vec2 	a_uv;

uniform 	mat4 	u_matMVP;
uniform 	mat4 	u_matWorld;
uniform 	float 	u_alpha;
uniform		vec2	u_CurrentFrame;
uniform		float	u_NumberFrame;
uniform		float	u_NumberSprite;

varying 	vec2 	v_uv;

void main()
{
	vec4 posL = vec4(a_posL, u_alpha);
	gl_Position = u_matMVP * posL;
	v_uv = vec2(a_uv.x * (1.0/u_NumberSprite) + u_CurrentFrame.x/u_NumberSprite, 1.0 - (a_uv.y * (1.0/u_NumberFrame) + u_CurrentFrame.y/u_NumberFrame));
}
   