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
    float roughness;
    float subsurface;
    float sheen;
    float sheenTint;
};
uniform Material material;

float pow5(float n);
vec3 sRGBToLinear(vec3 rgb);

vec3 BRDF();
vec3 BRDF_diffuse();
float Fresnel_diffuse();
float Subsurface();
vec3 Sheen();

vec3 BRDF_specular();

const float PI = 3.1415926;

// the dot product between
// I: incoming dir
// O: outgoing dir
// N: normal
// H: halfway dir 
float cosIN;
float cosON;
float cosIO;
float cosIH;
float cosNH;

vec3 baseColor;

void main() {
    vec3 normal = texture(material.texture_normal, f_in.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    vec3 lightColor = vec3(1, 1, 1);
    baseColor = texture(material.texture_diffuse, f_in.texCoords).rgb;

    vec3 lightDir = normalize(-f_in.tangentLightDir);
    vec3 eyeDir = normalize(f_in.tangentEyePos - f_in.tangentPos);
    vec3 halfwayDir = normalize(lightDir + eyeDir);
    cosIN = dot(lightDir, normal);
    cosON = dot(eyeDir, normal);
    cosIO = dot(lightDir, eyeDir);
    cosIH = dot(lightDir, halfwayDir);
    cosNH = dot(normal, halfwayDir);

    

    float diff = max(cosIN, 0.0);
    vec3 diffuse = BRDF() * lightColor * diff;

    FragColor = vec4(diffuse, 1);
}

float pow5(float n) {
    float n2 = n * n;
    return n2 * n2 * n;
}

vec3 sRGBToLinear(vec3 rgb)
{
    vec3 linearPart = rgb * (1.0 / 12.92);
    vec3 expPart    = pow((rgb + 0.055) * (1.0 / 1.055), vec3(2.4));
    vec3 mask       = vec3(lessThanEqual(rgb, vec3(0.04045)));
    return mix(expPart, linearPart, mask);
}

vec3 BRDF() {
    return BRDF_diffuse() + BRDF_specular();
}

vec3 BRDF_diffuse() {
    float fd = Fresnel_diffuse();
    float ss = Subsurface();
    return (baseColor / PI) * mix(fd, ss, material.subsurface) + Sheen();
}

float Fresnel_diffuse() {
    float f90 = 0.5 + 2 *(material.roughness * cosIH * cosIH);
    float f0 = 1;
    return mix(f0, f90, pow5(1 - cosIN)) * mix(f0, f90, pow5(1 - cosON));
}

float Subsurface() {
    float fss90 = material.roughness * cosIH * cosIH;
    float fss0 = 1;
    float fss = mix(fss0, fss90, pow5(1 - cosIN)) * mix(fss0, fss90, pow5(1 - cosON));
    return 1.25 * (fss * (1 / (cosIN + cosON) - 0.5) + 0.5);
}

vec3 Sheen() {
    vec3 linearColor = sRGBToLinear(baseColor);
    float luminance = 0.6 * linearColor.x + 0.3 * linearColor.y + 0.1 * linearColor.z;
    vec3 colorTint = luminance > 0 ? linearColor/luminance : vec3(1);
    vec3 colorSheen = mix(vec3(1), colorTint, material.sheenTint);
    return material.sheen * colorSheen * pow5(1 - cosIH);
}

vec3 BRDF_specular() {
    return vec3(0);
}