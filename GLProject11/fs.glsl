#version 330
in vec3 ex_Color;
out vec4 gl_FragColor;

uniform vec4 plane_color;

void main(void)
{
gl_FragColor = plane_color;
}