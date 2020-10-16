#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 vertex_color;

void main()
{
    vertex_color = col;
    gl_Position  = vec4(pos.x, pos.y, pos.z, 1.0);
}