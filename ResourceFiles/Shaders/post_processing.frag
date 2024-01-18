// #version 460 core
//
// in vec2 TexCoords;
// out vec4 FragColor;
//
// layout(binding = 0) uniform sampler2D image;
// layout(binding = 1) uniform sampler2D bloomTexture;
//
// uniform float gamma;
// uniform float exposure;
//
// void main()
// {
//     vec3 fragment = texture(image, TexCoords).rgb;
//     vec3 bloom = texture(bloomTexture, TexCoords).rgb;
//
//     vec3 color = fragment + bloom;
//
//     vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);
//     FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
// }

#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D image;
layout(binding = 1) uniform sampler2D bloomTexture;

uniform float gamma;
uniform float exposure;

// Положение источника света (в нормализованных координатах)
uniform vec2 lightPosition;

void main()
{
    // Получаем цвет из исходной текстуры
    vec3 fragment = texture(image, TexCoords).rgb;

    // Получаем цвет из текстуры bloom
    vec3 bloom = texture(bloomTexture, TexCoords).rgb;

    // Расстояние от текущей точки до источника света
    float distanceToLight = distance(TexCoords, lightPosition);

    // Интенсивность света убывает по мере увеличения расстояния
    float intensity = 1.0 - smoothstep(0.0, 0.1, distanceToLight);

    // Цвет ореола - например, белый
    vec3 haloColor = vec3(1.0);

    // Создаем текстуру для ореола света
    vec3 halo = intensity * haloColor;

    // Комбинируем цвета с эффектом bloom и ореолом света
    vec3 color = fragment + bloom + halo;

    // Применяем тоновую коррекцию и гамма-коррекцию
    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);
    FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
}
