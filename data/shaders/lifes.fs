
uniform vec4 u_color;
uniform int lifes;

varying vec2 v_uv;

void main()
{
	vec4 my_color = u_color;
	float n_lifes = float(lifes)/5;
	if(v_uv.x > n_lifes)
		my_color = my_color*0.2;

	gl_FragColor = my_color;
}
