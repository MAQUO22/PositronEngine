#version 460 core

in vec3 texCoords;

layout(binding=0) uniform samplerCube skybox;

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;

void main()
{
    frag_color = texture(skybox, texCoords);

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 1.0f) {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
