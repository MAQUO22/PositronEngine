#version 460 core

#define MAX_LIGHT_SOURCES 5

// uniforms
uniform vec3 direction_light_color;

uniform float ambient_factor;
uniform float diffuse_factor;
uniform float shininess;
uniform float specular_factor;

uniform int number_of_point_lights;

layout(binding=0) uniform sampler2D in_texture;
layout(binding=1) uniform sampler2D specular_map;
layout(binding=2) uniform sampler2D normal_map;
layout(binding=3) uniform sampler2D height_map;

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 tex_coord;
in vec3 camera_position;
in vec3 light_direction;

in vec3 point_light_positions[MAX_LIGHT_SOURCES];
in vec3 point_light_colors[MAX_LIGHT_SOURCES];

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;


vec4 directionLight(vec3 light_direction_p, vec3 direction_light_color_p)
{
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 ambient = ambient_factor * direction_light_color_p;

    vec3 normal = normalize(texture(normal_map, tex_coord).xyz * 2.0 - 1.0);
    //normal = normalize(frag_normal);

    vec3 _light_direction = normalize(-light_direction_p);
    float diffuse_factor_modified = max(dot(normal, _light_direction), 0.0);
    vec3 diffuse = diffuse_factor * direction_light_color_p * diffuse_factor_modified;

    vec3 reflect_direction = reflect(-_light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_factor * specular_value * direction_light_color_p;

    vec4 diffuse_texture = texture(in_texture, tex_coord);
    vec4 specular_texture = texture(specular_map, tex_coord);

    return vec4(ambient + diffuse, 1.0) * diffuse_texture + specular_texture.r * vec4(specular,1.0f);
}

vec4 pointLight(vec3 light_position_p, vec3 point_light_color_p)
{
    vec3 ambient = ambient_factor * point_light_color_p;

    vec3 light_vector = light_position_p - frag_position;
    float dist = length(light_vector);
    float a = 0.05f;
    float b = 0.01f;
    float intens = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 light_direction_ = normalize(light_vector);

    vec3 normal = normalize(texture(normal_map, tex_coord).xyz * 2.0 - 1.0);

    //vec3 normal = normalize(frag_normal);
    float diffuse_factor_modified = max(dot(normal, light_direction_), 0.0);
    vec3 diffuse = diffuse_factor * point_light_color_p * diffuse_factor_modified;

    vec3 specular;

    if(diffuse_factor_modified != 0.0f)
    {
        vec3 view_direction = normalize(camera_position - frag_position);

        vec3 halfway_vector = normalize(view_direction + light_direction_);

        vec3 reflect_direction = reflect(-light_direction_, normal);
        float specular_value = pow(max(dot(normal, halfway_vector), 0.0), shininess);
        specular = specular_factor * specular_value * point_light_color_p;
    }

    vec4 diffuse_texture = texture(in_texture, tex_coord);
    vec4 specular_texture = texture(specular_map, tex_coord);

    return vec4(ambient + diffuse * intens, 1.0) * diffuse_texture + specular_texture.r * vec4(specular * intens,1.0f);
}

void main() {

    frag_color = directionLight(light_direction, direction_light_color);

    for(int i = 0; i < number_of_point_lights; i++)
    {
        frag_color += pointLight(point_light_positions[i], point_light_colors[i]);
    }

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 0.6f) {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

