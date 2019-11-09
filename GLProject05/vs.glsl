#version 330
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Color;
out vec3 ex_Color;

uniform mat4 modelTransform;
uniform mat4 projTransform;
void main(void )
{
gl_Position = modelTransform * vec4 (in_Position , 1.0);
ex_Color = in_Color;
}