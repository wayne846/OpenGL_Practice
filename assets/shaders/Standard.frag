#version 330 core
out vec4 FragColor;

in V_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoords;

    vec3 tangentLightDir;
    vec3 tangentEyePos;
    vec3 tangentPos;
} f_in;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_normal;
};
uniform Material material;

void main()
{
    vec3 normal = texture(material.texture_normal, f_in.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 baseColor = texture(material.texture_diffuse, f_in.texCoords).rgb;
    float diff = max(dot(normalize(-f_in.tangentLightDir), normal), 0.0);
    vec3 diffuse = diff * baseColor;
    vec3 ambient = vec3(0.3, 0.3, 0.3) * baseColor;

    FragColor = vec4(ambient + diffuse, 1);
}