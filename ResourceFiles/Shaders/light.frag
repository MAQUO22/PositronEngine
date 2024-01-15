#version 460

// uniforms
uniform vec3 light_color;
layout(binding=0) uniform sampler2D in_texture;

// varyings (input)
in vec3 esNormal;
in vec2 texCoord;

// output
out vec4 frag_color;

void main() {
    // Применяем эффект bloom к текстуре
    vec4 bloom = vec4(0.0);

    // Применяем размытие Гаусса к bloom
    float blurSize = 5.0; // Размер размытия
    float sigma = 2.0; // Параметр Гауссовой функции

    for (float x = -blurSize; x <= blurSize; x += 1.0) {
        for (float y = -blurSize; y <= blurSize; y += 1.0) {
            bloom += texture(in_texture, texCoord + vec2(x, y) / 800.0) *
                    exp(-(x*x + y*y) / (2.0 * sigma * sigma)) / (2.0 * 3.14159 * sigma * sigma);
        }
    }

    // Сложение с исходной текстурой (in_texture)
    vec4 final_color = texture(in_texture, texCoord) + bloom;

    // Настраиваемый коэффициент для усиления эффекта bloom для солнца
    float bloomIntensity = 1.0;
    frag_color = final_color * vec4(light_color * bloomIntensity, 1.0);
}
