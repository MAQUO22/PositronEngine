#version 460

// uniforms
uniform vec3 light_color;
layout(binding=0) uniform sampler2D in_texture;

// varyings (input)
in vec3 esNormal;
in vec2 texCoord;

// output
out vec4 frag_color;

void main()
{
    frag_color = texture(in_texture, texCoord) * vec4(light_color, 1.0);
}
