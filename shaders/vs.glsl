#version 330 core

layout (location = 0) in vec2 vsIn_Position;

void main()
{
    gl_Position = vec4(vsIn_Position, 0, 1);
}
