#version 460 core

// uniforms
uniform vec3 light_color;
uniform float ambient_factor;
uniform float diffuse_factor;
uniform float shininess;
uniform float specular_factor;

layout(binding=0) uniform sampler2D in_texture;
layout(binding=1) uniform sampler2D specular_map;
layout(binding=2) uniform sampler2D normal_map;
layout(binding=3) uniform sampler2D height_map;

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 texCoord;
in vec3 camera_position;
in vec3 light_position;

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;


vec4 directionLight()
{
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 ambient = ambient_factor * light_color;

    vec3 normal = normalize(texture(normal_map, texCoord).xyz * 2.0 - 1.0);
    //normal = normalize(frag_normal);

    vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse_factor_modified = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_factor * light_color * diffuse_factor_modified;

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_factor * specular_value * light_color;

    vec4 diffuse_texture = texture(in_texture, texCoord);
    vec4 specular_texture = texture(specular_map, texCoord);

    return vec4(ambient + diffuse, 1.0) * diffuse_texture + specular_texture.r * vec4(specular,1.0f);
}

vec4 pointLight()
{
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 ambient = ambient_factor * light_color;

    vec3 light_vector = light_position - frag_position;
    float dist = length(light_vector);
    float a = 0.05f;
    float b = 0.01f;
    float intens = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 light_direction = normalize(light_vector);

    vec3 normal = normalize(texture(normal_map, texCoord).xyz * 2.0 - 1.0);

    //vec3 normal = normalize(frag_normal);
    float diffuse_factor_modified = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_factor * light_color * diffuse_factor_modified;

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_factor * specular_value * light_color;

    vec4 diffuse_texture = texture(in_texture, texCoord);
    vec4 specular_texture = texture(specular_map, texCoord);

    return vec4(ambient + diffuse * intens, 1.0) * diffuse_texture + specular_texture.r * vec4(specular * intens,1.0f);
}

void main() {

    frag_color = pointLight();

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 0.6f) {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

