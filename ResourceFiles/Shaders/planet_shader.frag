#version 460

// uniforms
uniform vec3 light_color;
uniform vec3 light_position;
uniform float ambient_factor;
uniform float diffuse_factor;
uniform vec3 camera_position;
uniform bool atphmosphere;

layout(binding=0) uniform sampler2D in_texture;         // Земля
layout(binding=1) uniform sampler2D in_cloud_texture;   // Облака
layout(binding=2) uniform sampler2D in_night_texture;   // Ночная текстура

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 texCoordGround;
in vec2 texCoordCloud;

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;

void main() {
    // ambient
    vec3 ambient = ambient_factor * light_color;

    // diffuse
    vec3 normal = normalize(frag_normal);
    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse_factor_modified = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_factor * light_color * diffuse_factor_modified;

    // specular
    //vec3 view_dir = normalize(camera_position * frag_position);
    //vec3 reflect_dir = reflect(light_direction, normal);
    //float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), 0.9);
    //vec3 specular = 0.9 * specular_value * light_color;
    vec3 specular = vec3(0.0f);

    if(atphmosphere)
    {
        vec4 ground_color;
        if (diffuse_factor_modified > 0.0) {
            ground_color = texture(in_texture, texCoordGround);
        } else {
            // Применяем ночную текстуру с ярче
            vec4 night_color = texture(in_night_texture, texCoordGround);
            ground_color = mix(vec4(1.0), night_color, 1) * 7.0f; // Меняем 0.5 на нужный вам коэффициент смягчения
        }

        // Цвет для облаков (белый)
        vec4 cloud_color = texture(in_cloud_texture, texCoordCloud);

        // Голубой цвет для озонового слоя
        vec3 ozone_color = vec3(0.0, 0.3, 0.7);

        // Смешиваем цвета текстур для земли и облаков
        vec3 final_color = mix(ground_color.rgb, vec3(1.0), cloud_color.r);

        // Добавляем голубой цвет в область, где свет попадает на поверхность Земли
        final_color += mix(vec3(0.0), ozone_color, diffuse_factor_modified);

        frag_color = vec4(ambient + diffuse + specular, 1.0) * vec4(final_color, 1.0);
    }
    else
    {
        vec4 ground_color = texture(in_texture, texCoordGround);
        frag_color = vec4(ambient + diffuse + specular, 1.0) * ground_color;
    }

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 1.5f)
    {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else
    {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
