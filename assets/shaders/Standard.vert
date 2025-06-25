#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoords;

layout (std140) uniform Matrices{
    mat4 view;
    mat4 projection;
};

out V_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoords;

    vec3 tangentLightDir;
    vec3 tangentEyePos;
    vec3 tangentPos;
} v_out;

uniform mat4 model;

uniform vec3 lightDir;
uniform vec3 eyePos;

void main()
{
    v_out.normal = mat3(transpose(inverse(model))) * normal;
    v_out.position = vec3(model * vec4(position, 1)); 
    v_out.texCoords = texCoords;

    // calculate tangent space
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    v_out.tangentLightDir = TBN * lightDir;
    v_out.tangentEyePos  = TBN * eyePos;
    v_out.tangentPos  = TBN * v_out.position;

    gl_Position = projection * view * model * vec4(position, 1);
}