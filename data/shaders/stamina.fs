
uniform vec4 u_color;
uniform float stamina;

varying vec2 v_uv;

void main()
{
	vec4 my_color = u_color;
	float n_stamina = float(stamina)/50.0;
	if(v_uv.x>n_stamina)
		my_color = my_color*0.0;

	gl_FragColor = my_color;
}
