#version 460

// uniforms
uniform vec3 light_color;
uniform vec3 light_position;
uniform float ambient_factor;
uniform float diffuse_factor;
layout(binding=0) uniform sampler2D in_texture;

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 texCoordGround;

// output
out vec4 frag_color;

void main() {
    // ambient
    vec3 ambient = ambient_factor * light_color;

    // diffuse
    vec3 normal = normalize(frag_normal);
    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse_factor_modified = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_factor * light_color * diffuse_factor_modified;

    // specular
    vec3 specular = vec3(0.0);

    vec4 ground_color = texture(in_texture, texCoordGround);

    frag_color = vec4(ambient + diffuse + specular, 1.0) * ground_color;
}
