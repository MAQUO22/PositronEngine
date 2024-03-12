#version 460

// uniforms
uniform vec3 light_color;
uniform vec3 light_position;
uniform float ambient_factor;
uniform float diffuse_factor;
uniform vec3 camera_position;
uniform float shininess;
uniform float specular_factor;

layout(binding=0) uniform sampler2D in_texture;
layout(binding=1) uniform sampler2D specular_map;
//layout(binding=2) uniform sampler2D normal_map;

// varyings (input)
in vec3 frag_normal;
in vec3 frag_position;
in vec2 texCoord;

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;

void main() {

    vec3 view_direction = normalize(camera_position - frag_position);

//     float heightScale = 0.05f;
//     const float minLayers = 4.0f;
//     const float maxLayers = 32.0f;
//     float numLayers = mix(maxLayers,minLayers, abs(dot(vec3(0.0f,0.0f,1.0f), view_direction)));
//     float layerDepth = 1.0f;
//     float currentLayerDepth = 0.0f;
//
//     vec2 S = view_direction.xy / view_direction.z * heightScale;
//     vec2 deltaUVs = S / numLayers;
//
//     vec2 UVs = texCoord;
//     float currentDepthMapValue = 1.0f - texture(height_map, UVs).r;
//
//     while(currentLayerDepth < currentDepthMapValue)
//     {
//         UVs -= deltaUVs;
//         currentDepthMapValue = 1.0f - - texture(height_map, UVs).r;
//         currentLayerDepth += layerDepth;
//     }
//
//     vec2 prevTexCoords = UVs + deltaUVs;
//     float afterDepth = currentDepthMapValue - currentLayerDepth;
//     float beforeDepth = 1.0f - texture(height_map, prevTexCoords).r - currentLayerDepth + layerDepth;
//     float weight = afterDepth / (afterDepth - beforeDepth);
//     UVs = prevTexCoords * weight + UVs * (1.0f - weight);
//
//     if(UVs.x > 1.0f || UVs.y > 1.0f || UVs.x < 0.0f || UVs.y < 0.0f)
//         discard;

    // ambient
    vec3 ambient = ambient_factor * light_color;

    // diffuse
    vec3 normal = normalize(frag_normal);

    //vec3 normal = texture(normal_map, texCoord).xyz;
    //normal = normalize(normal * 2.0 - 1.0);

    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse_factor_modified = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_factor * light_color * diffuse_factor_modified;

    // specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_factor * specular_value * light_color;

    vec4 diffuse_texture = texture(in_texture, texCoord);
    vec4 specular_texture = texture(specular_map, texCoord);

    frag_color = vec4(ambient + diffuse, 1.0) * diffuse_texture + specular_texture.r * vec4(specular,1.0f);

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 0.6f)
    {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else
    {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
