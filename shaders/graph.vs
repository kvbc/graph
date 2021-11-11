#version 450 core
layout (location = 0) in vec2 aPos;

layout(std140, binding = 0) uniform Camera
{
    mat4 projection;
};

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
