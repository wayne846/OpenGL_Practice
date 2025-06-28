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
    sampler2D texture_baseColor;
    sampler2D texture_normal;
    sampler2D texture_roughness;

    vec3 baseColor;
    float roughness;
    float subsurface;
    float sheen;
    float sheenTint;
    float anisotropic;
    float specular;
    float specularTint;
    float metallic;
    float clearcoat;
    float clearcoatGloss;

    bool useBaseColorMap;
    bool useNormalMap;
    bool useRoughnessMap;
};
uniform Material material;

float pow5(float n);
float pow2(float n);
vec3 sRGBToLinear(vec3 rgb);

vec3 BRDF();
vec3 BRDF_diffuse();
float Fresnel_diffuse();
float Subsurface();
vec3 Sheen();
vec3 BRDF_specular();
float Distribution_specular();
vec3 Fresnel_specular();
float Geometry_specular();
float BRDF_clearcoat();
float Distribution_clearcoat();
float Fresnel_clearcoat();
float Geometry_clearcoat();

float GGX_anisotropic();
float smith_GGX_anisotropic(float cosThetaN, float cosThetaX, float cosThetaY);
float GTR1(float a);
float smithG_GGX(float cosThetaN, float alphaG);

const float PI = 3.1415926;

// the dot product between
// I: incoming dir
// O: outgoing dir
// N: normal
// H: halfway dir 
// X: tangent
// Y: bitangent
float cosIN;
float cosON;
float cosIO;
float cosIH;
float cosNH;
float cosXH;
float cosYH;
float cosIX;
float cosIY;
float cosOX;
float cosOY;

vec3 baseColor;
vec3 linearColor;
vec3 colorTint;

float roughness;

float ax;
float ay;

void main() {
    // normal
    vec3 normal;
    if (material.useNormalMap) {
        normal = texture(material.texture_normal, f_in.texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    } else {
        normal = vec3(0, 0, 1);
    }

    // base color
    if (material.useBaseColorMap) {
        baseColor = texture(material.texture_baseColor, f_in.texCoords).rgb;
    } else {
        baseColor = material.baseColor;
    }
    linearColor = baseColor;
    float luminance = 0.6 * linearColor.x + 0.3 * linearColor.y + 0.1 * linearColor.z;
    colorTint = luminance > 0 ? linearColor/luminance : vec3(1);

    // roughness
    if (material.useRoughnessMap) {
        roughness = texture(material.texture_roughness, f_in.texCoords).r;
    } else {
        roughness = material.roughness;
    }

    // dot product
    vec3 lightDir = normalize(-f_in.tangentLightDir);
    vec3 eyeDir = normalize(f_in.tangentEyePos - f_in.tangentPos);
    vec3 halfwayDir = normalize(lightDir + eyeDir);
    cosIN = dot(lightDir, normal);
    cosON = dot(eyeDir, normal);
    cosIO = dot(lightDir, eyeDir);
    cosIH = dot(lightDir, halfwayDir);
    cosNH = dot(normal, halfwayDir);
    // dot product about tangent
    vec3 tangent = vec3(1, 0, 0);
    vec3 bitangent_ortho = normalize(cross(normal, tangent));
    vec3 tangent_ortho  = cross(bitangent_ortho, normal);
    cosXH = dot(tangent_ortho, halfwayDir);
    cosYH = dot(bitangent_ortho, halfwayDir);
    cosIX = dot(lightDir, tangent_ortho);
    cosIY = dot(lightDir, bitangent_ortho);
    cosOX = dot(eyeDir, tangent_ortho);
    cosOY = dot(eyeDir, bitangent_ortho);

    // aspect for anisotropic
    float aspect = sqrt(1 - material.anisotropic * 0.9);
    ax = max(0.001, pow2(roughness) / aspect);
    ay = max(0.001, pow2(roughness) * aspect);

    vec3 lightColor = vec3(3, 3, 3);
    float diff = max(cosIN, 0.0);
    vec3 diffuse = BRDF() * lightColor * diff;

    FragColor = vec4(diffuse, 1);
}

float pow5(float n) {
    float n2 = n * n;
    return n2 * n2 * n;
}

float pow2(float n) {
    return n * n;
}

vec3 sRGBToLinear(vec3 rgb)
{
    vec3 linearPart = rgb * (1.0 / 12.92);
    vec3 expPart = pow((rgb + 0.055) * (1.0 / 1.055), vec3(2.4));
    vec3 mask = vec3(lessThanEqual(rgb, vec3(0.04045)));
    return mix(expPart, linearPart, mask);
}

vec3 BRDF() {
    if(cosIN < 0) return vec3(0);
    return BRDF_diffuse() * (1 - material.metallic) + BRDF_specular() + BRDF_clearcoat();
}

vec3 BRDF_diffuse() {
    float fd = Fresnel_diffuse();
    float ss = Subsurface();
    return (linearColor / PI) * mix(fd, ss, material.subsurface) + Sheen();
}

float Fresnel_diffuse() {
    float f90 = 0.5 + 2 *(roughness * cosIH * cosIH);
    float f0 = 1;
    return mix(f0, f90, pow5(1 - cosIN)) * mix(f0, f90, pow5(1 - cosON));
}

float Subsurface() {
    float fss90 = roughness * cosIH * cosIH;
    float fss0 = 1;
    float fss = mix(fss0, fss90, pow5(1 - cosIN)) * mix(fss0, fss90, pow5(1 - cosON));
    return 1.25 * (fss * (1 / (cosIN + cosON) - 0.5) + 0.5);
}

vec3 Sheen() {
    vec3 colorSheen = mix(vec3(1), colorTint, material.sheenTint);
    return material.sheen * colorSheen * pow5(1 - cosIH);
}

vec3 BRDF_specular() {
    float d = Distribution_specular();
    vec3 f = Fresnel_specular();
    float g = Geometry_specular();
    return (d * f * g);// / (4 * cosIN * cosON);
}

float Distribution_specular() {
    return GGX_anisotropic();
}

vec3 Fresnel_specular() {
    vec3 specularColor0 = mix(material.specular * 0.08 * mix(vec3(1), colorTint, material.specularTint), linearColor, material.metallic);
    return mix(specularColor0, vec3(1), pow5(cosIH));
}

float Geometry_specular() {
    return smith_GGX_anisotropic(cosIN, cosIX, cosIY) * smith_GGX_anisotropic(cosON, cosOX, cosOY);
}

float GGX_anisotropic() {
    return 1.0 / (PI * ax * ay * pow2(pow2(cosXH / ax) + pow2(cosYH / ay) + pow2(cosNH)));
}

float smith_GGX_anisotropic(float cosThetaN, float cosThetaX, float cosThetaY) {
    return 1 / (cosThetaN + sqrt(pow2(cosThetaX * ax) + pow2(cosThetaY * ay) + pow2(cosThetaN)));
}

float BRDF_clearcoat() {
    return 0.25 * material.clearcoat * Distribution_clearcoat() * Fresnel_clearcoat() * Geometry_specular();
}

float Distribution_clearcoat() {
    return GTR1(mix(0.1, 0.001, material.clearcoatGloss));
}

float Fresnel_clearcoat() {
    return mix(.04, 1.0, pow5(cosIH));
}

float Geometry_clearcoat() {
    return smithG_GGX(cosIN, .25) * smithG_GGX(cosON, .25);
}

float GTR1(float a)
{
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*cosNH*cosNH;
    return (a2-1) / (PI*log(a2)*t);
}

float smithG_GGX(float cosThetaN, float alphaG){
    float a = alphaG*alphaG;
    float b = cosThetaN*cosThetaN;
    return 1 / (cosThetaN + sqrt(a + b - a*b));
}