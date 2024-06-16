varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
    // Oscillate brightness between 0.5 and 1.5
    float brightness = 0.75 + 0.25 * sin(u_time * 7.0);

    // Fetch the texture color and apply brightness
    vec4 texColor = texture2D(u_texture, v_uv);
    gl_FragColor = u_color * texColor * brightness;
}
