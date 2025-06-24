#version 330 core
out vec4 FragColor;

in V_OUT {
    vec3 position;
    vec3 normal;
} f_in;

void main()
{
    vec3 norm = normalize(f_in.normal);
    vec3 lightDir = vec3(1, 1, 0);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffColor = diff * vec3(1, 1, 1);
    FragColor = vec4(norm, 1);

    vec3 n = normalize(f_in.normal);
    // ±N (-1¡÷+1) ¬M®g¨ì (0¡÷1)
    vec3 color = n * 0.5 + 0.5;  
    FragColor = vec4(color, 1.0);
}