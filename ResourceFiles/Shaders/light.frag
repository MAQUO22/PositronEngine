#version 460

// uniforms
uniform vec3 light_color;

// varyings (input)
in vec3 esNormal;
in vec2 texCoord;

// output
out vec4 frag_color;

void main()
{
    frag_color = vec4(light_color, 1.0);
}
