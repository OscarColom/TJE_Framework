#version 120

varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

uniform vec3 u_camera_pos;
uniform vec3 u_light_pos;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;


void main()
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_pos - v_world_position);
	vec3 V = normalize(u_camera_pos - v_world_position);
	vec3 R = normalize(reflect(-L,V));

	vec3 Ia = vec3(0.6,0.6,0.6);
	vec3 Id = vec3(1,1,1);
	vec3 Is = vec3(1,1,1);

	vec3 ambient = u_Ka * Ia * u_Kd;
	vec3 diffuse = u_Kd * max(dot(L,N), 0.0) * Id;
	vec3 specular = u_Ks * pow(max(dot(R,V), 0.0), 50.0) * Is;

	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
