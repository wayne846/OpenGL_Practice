#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Matrices{
    mat4 view;
    mat4 projection;
};

out V_OUT {
    vec3 position;
    vec3 normal;
} v_out;

void main()
{
    v_out.normal = normal;
    v_out.position = position;

    gl_Position = projection * view * vec4(position, 1);
}