#version 460

// uniforms
uniform vec3 light_color;
uniform bool textureAvailable;

layout(binding=0) uniform sampler2D in_texture;

// varyings (input)
in vec3 esNormal;
in vec2 texCoord;

// output
out vec4 frag_color;

void main()
{
    if (textureAvailable)
    {
        vec4 texColor = texture(in_texture, texCoord);
        frag_color = texColor * vec4(light_color, 1.0);
    }
    else
    {
        frag_color = vec4(light_color, 1.0);
    }
}
