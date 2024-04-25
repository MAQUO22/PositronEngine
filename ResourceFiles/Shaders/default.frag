#version 460 core

#define MAX_LIGHT_SOURCES 4
// uniforms
uniform vec3 direction_light_color;

uniform vec4 base_color;

uniform bool has_color_map;
uniform bool has_normal_map;
uniform bool has_roughness_map;
uniform bool has_displacement_map;

uniform float ambient_factor;
uniform float diffuse_factor;

uniform float metallic;
uniform float specular_factor;
uniform float roughness;

uniform int number_of_point_lights;
uniform int number_of_spot_lights;

uniform vec3 point_light_colors[MAX_LIGHT_SOURCES];
uniform vec3 spot_light_colors[MAX_LIGHT_SOURCES];

uniform float constant_attenuation[MAX_LIGHT_SOURCES];
uniform float linear_attenuation[MAX_LIGHT_SOURCES];

uniform float outer_cone[MAX_LIGHT_SOURCES];
uniform float inner_cone[MAX_LIGHT_SOURCES];

layout(binding=0) uniform sampler2D color_map;
layout(binding=1) uniform sampler2D roughness_map;
layout(binding=2) uniform sampler2D normal_map;
layout(binding=3) uniform sampler2D shadow_map;
layout(binding=4) uniform samplerCube point_light_shadow_cubemap_array[MAX_LIGHT_SOURCES];
layout(binding=9) uniform sampler2D spot_light_shadow_map_array[MAX_LIGHT_SOURCES];

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 tex_coord;
in vec3 camera_position;

in vec3 light_direction;

in vec3 point_light_positions[MAX_LIGHT_SOURCES];

in vec3 spot_light_positions[MAX_LIGHT_SOURCES];
in vec3 spot_light_direction[MAX_LIGHT_SOURCES];
in vec4 frag_position_spot_light[MAX_LIGHT_SOURCES];

in vec4 frag_position_light;

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;


vec4 directionLight(vec3 light_direction, vec3 direction_light_color, vec4 diffuse_texture, float roughness, vec3 normal)
{

    vec3 view_direction = normalize(camera_position - frag_position);

    vec3 _light_direction = normalize(light_direction);
    float diffuse_factor_modified = max(dot(normal, _light_direction), 0.0f);
    vec3 diffuse = diffuse_factor * direction_light_color * diffuse_factor_modified;

    vec3 reflect_direction = reflect(-_light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), roughness);
    vec3 specular = specular_factor * specular_value * direction_light_color;

    float shadow = 0.0f;
    // Sets lightCoords to cull space
    vec3 lightCoords = frag_position_light.xyz / frag_position_light.w;
    if(lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.025f * (1.0f - dot(normal, _light_direction)), 0.0005f);

        // Smoothens out the shadows
        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadow_map, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadow_map, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    vec4 material_color = mix(diffuse_texture, vec4(0.0f, 0.0f, 0.0f, 1.0f), metallic);

    return vec4(ambient_factor + diffuse * (1.0f - shadow), 1.0) * material_color +
                vec4(specular * (1.0f - shadow), 1.0f);

}

vec4 pointLight(vec3 light_position, vec3 point_light_color, float constant_attenuation,
                float linear_attenuation, samplerCube shadow_map_point, vec4 diffuse_texture, float roughness, vec3 normal)
{
    vec3 light_vector = light_position - frag_position;
    float dist = length(light_vector);

    float intens = 1.0f / (constant_attenuation * dist * dist + linear_attenuation * dist + 1.0f);

    vec3 light_direction_ = normalize(light_vector);

//     vec3 normal = normalize(texture(normal_map, tex_coord).xyz * 2.0 - 1.0);

    //vec3 normal = normalize(frag_normal);
    float diffuse_factor_modified = max(dot(normal, light_direction_), 0.0);
    vec3 diffuse = diffuse_factor * point_light_color * diffuse_factor_modified;

    vec3 specular;

    if(diffuse_factor_modified != 0.0f)
    {
        vec3 view_direction = normalize(camera_position - frag_position);

        vec3 halfway_vector = normalize(view_direction + light_direction_);

        vec3 reflect_direction = reflect(-light_direction_, normal);
        float specular_value = pow(max(dot(normal, halfway_vector), 0.0), roughness);
        specular = specular_factor * specular_value * point_light_color;
    }

    float shadow = 0.0f;
    vec3 fragToLight = frag_position - light_position;
    float currentDepth = length(fragToLight);
    float bias = max(0.5f * (1.0f - dot(normal, light_direction_)), 0.0005f);

    // Not really a radius, more like half the width of a square
    int sampleRadius = 2;
    float offset = 0.03f;
    for(int z = -sampleRadius; z <= sampleRadius; z++)
    {
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadow_map_point, fragToLight + vec3(x, y, z) * offset).r;
                closestDepth *= 145.0f;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
    }
    // Average shadow
    shadow /= pow((sampleRadius * 2 + 1), 3);

    vec4 material_color = mix(diffuse_texture, vec4(0.0f, 0.0f, 0.0f, 1.0f), metallic);

    return vec4(diffuse * (1.0f - shadow) * intens, 1.0) * material_color +
                vec4(specular * (1.0f - shadow) * intens, 1.0f);

}

vec4 spotLight(vec3 light_position, vec3 spot_light_color, vec3 spot_light_direction,
               float outer_cone, float inner_cone, vec4 diffuse_texture, float roughness,
               vec4 frag_position_light, sampler2D shadow_map_spot, vec3 normal)
{

    vec3 view_direction = normalize(camera_position - frag_position);

    //vec3 normal = normalize(texture(normal_map, tex_coord).xyz * 2.0 - 1.0);
    //normal = normalize(frag_normal);

    vec3 _light_direction = normalize(light_position - frag_position);
    float diffuse_factor_modified = max(dot(normal, _light_direction), 0.0);
    vec3 diffuse = diffuse_factor * spot_light_color * diffuse_factor_modified;

    vec3 reflect_direction = reflect(-_light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), roughness);
    vec3 specular = specular_factor * specular_value * spot_light_color;

    float angle = dot(normalize(spot_light_direction), -_light_direction);

    float inten = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

    float shadow = 0.0f;
    // Sets lightCoords to cull space
    vec3 lightCoords = frag_position_light.xyz / frag_position_light.w;
    if(lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.0025f * (1.0f - dot(normal, _light_direction)), 0.00005f);

        // Smoothens out the shadows
        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadow_map_spot, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadow_map_spot, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    vec4 material_color = mix(diffuse_texture, vec4(0.0f, 0.0f, 0.0f, 1.0f), metallic);

    return vec4(diffuse * (1.0f - shadow) * inten, 1.0) * material_color + vec4(specular * (1.0f - shadow) * inten, 1.0f);
}


void main() {
    vec4 diffuse_texture;
    float roughness_value;
    vec3 normal;

    if (has_color_map)
        diffuse_texture = texture(color_map, tex_coord);
    else
        diffuse_texture = base_color;

    if (has_roughness_map)
        roughness_value = texture(roughness_map, tex_coord).r;
    else
        roughness_value = roughness;

    if(has_normal_map)
        normal = normalize(texture(normal_map, tex_coord).xyz * 2.0 - 1.0);
    else
        normal = normalize(frag_normal);

    frag_color = directionLight(light_direction, direction_light_color,
                                diffuse_texture, roughness_value, normal);

    for(int i = 0; i < number_of_point_lights; i++)
    {
        frag_color += pointLight(point_light_positions[i], point_light_colors[i],
                                 constant_attenuation[i], linear_attenuation[i],
                                 point_light_shadow_cubemap_array[i],
                                 diffuse_texture, roughness_value, normal);
    }

    for(int i = 0; i < number_of_spot_lights; i++)
    {
         frag_color += spotLight(spot_light_positions[i], spot_light_colors[i], spot_light_direction[i],
                                 outer_cone[i], inner_cone[i],
                                 diffuse_texture, roughness_value,
                                 frag_position_spot_light[i], spot_light_shadow_map_array[i], normal);
    }

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 3.0f) {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

