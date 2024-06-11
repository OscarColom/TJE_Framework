uniform vec4 u_color;
uniform int lifes;
uniform float u_time; // Añadimos tiempo como una variable uniforme

varying vec2 v_uv;

void main()
{
	vec4 my_color = u_color;
	float n_lifes = float(lifes) / 5.0;

	my_color.rgb *= 0.8;


	// Si las vidas son una, hacemos que la barra parpadee
	if (lifes == 1) {
		float blink = abs(sin(u_time * 5.0)); // Parpadeo basado en el tiempo
		my_color *= mix(0.2, 1.0, blink); // Interpolamos entre intensidad baja y normal
	}

	if (v_uv.x > n_lifes)
		my_color *= 0.2; // Mantener la parte vacía de la barra más oscura

	float gradient = smoothstep(0.4, 0.6, v_uv.y); // Crear un gradiente suave en el medio del eje Y
    my_color.rgb *= mix(1.0, 1.2, gradient); // Incrementar el brillo en el medio del eje Y

	gl_FragColor = my_color;
}
