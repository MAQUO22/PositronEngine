#version 460

// uniforms
uniform vec3 light_color;
layout(binding=0) uniform sampler2D in_texture;

// varyings (input)
in vec3 esNormal;
in vec2 texCoord;

// output
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bloom_color;

void main()
{
    frag_color = texture(in_texture, texCoord) * vec4(light_color, 1.0);

    float brightness = dot(frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));

    if(brightness > 0.5f)
    {
        bloom_color = vec4(frag_color.rgb, 1.0f);
    }
    else
    {
        bloom_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
