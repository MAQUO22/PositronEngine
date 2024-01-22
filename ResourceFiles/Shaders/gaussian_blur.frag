// #version 460 core
//
// out vec4 frag_color;
//
// in vec2 TexCoords;
//
// uniform sampler2D screen_texture;
// uniform bool horizontal;
// uniform float weight[9] = float[] (0.05f, 0.09f, 0.12f, 0.15f, 0.18f, 0.15f, 0.12f, 0.09f, 0.05f);
// uniform float blur_radius; // Новый параметр для радиуса размытия
// uniform float blur_factor; // Новый параметр для настройки степени размытия
//
// void main()
// {
//     vec2 tex_offset = 1.0f / textureSize(screen_texture, 0);
//     vec3 result = texture(screen_texture, TexCoords).rgb * weight[0];
//
//     for(int i = 1; i < 7; i++)
//     {
//         float offset = float(i) * blur_radius;
//
//         if(horizontal)
//         {
//             result += texture(screen_texture, TexCoords + vec2(tex_offset.x * offset, 0.0)).rgb * weight[i] * blur_factor;
//             result += texture(screen_texture, TexCoords - vec2(tex_offset.x * offset, 0.0)).rgb * weight[i] * blur_factor;
//         }
//         else
//         {
//             result += texture(screen_texture, TexCoords + vec2(0.0, tex_offset.y * offset)).rgb * weight[i] * blur_factor;
//             result += texture(screen_texture, TexCoords - vec2(0.0, tex_offset.y * offset)).rgb * weight[i] * blur_factor;
//         }
//     }
//
//     frag_color = vec4(result, 1.0f);
// }


#version 460 core

out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D screen_texture;
uniform bool horizontal;
uniform float weight[25] = float[] (
    0.023246, 0.033594, 0.039317, 0.033594, 0.023246,
    0.033594, 0.049045, 0.057223, 0.049045, 0.033594,
    0.039317, 0.057223, 0.067450, 0.057223, 0.039317,
    0.033594, 0.049045, 0.057223, 0.049045, 0.033594,
    0.023246, 0.033594, 0.039317, 0.033594, 0.023246
);
uniform float baseBlurRadius; // Базовый радиус размытия
uniform float baseBlurFactor; // Базовая интенсивность размытия
uniform float cameraDistance;  // Расстояние от камеры
uniform float blurDistanceFactor;
float dynamicBlurRadius = 0.0;
float dynamicBlurFactor = 0.0;

const float maxBlurDistance = 25.0; // Максимальное расстояние, при котором не увеличивается размытие


void main()
{
    vec2 tex_offset = 1.0 / textureSize(screen_texture, 0);
    vec3 result = texture(screen_texture, TexCoords).rgb * weight[12];

    if(cameraDistance < maxBlurDistance)
    {
        dynamicBlurRadius = baseBlurRadius * cameraDistance * blurDistanceFactor;
        dynamicBlurFactor = baseBlurFactor * cameraDistance * blurDistanceFactor;
    }
    else
    {
        dynamicBlurRadius = 1.8;
        dynamicBlurFactor = 0.7812;
    }

    for(int i = 1; i < 8; i++)
    {
        float offset = float(i) * dynamicBlurRadius;

        if(horizontal)
        {
            result += texture(screen_texture, TexCoords + vec2(tex_offset.x * offset, 0.0)).rgb * weight[i] * dynamicBlurFactor;
            result += texture(screen_texture, TexCoords - vec2(tex_offset.x * offset, 0.0)).rgb * weight[i] * dynamicBlurFactor;
        }
        else
        {
            result += texture(screen_texture, TexCoords + vec2(0.0, tex_offset.y * offset)).rgb * weight[i] * dynamicBlurFactor;
            result += texture(screen_texture, TexCoords - vec2(0.0, tex_offset.y * offset)).rgb * weight[i] * dynamicBlurFactor;
        }
    }

    frag_color = vec4(result, 1.0);
}




















