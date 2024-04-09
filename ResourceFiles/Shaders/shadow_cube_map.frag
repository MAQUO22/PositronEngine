#version 460 core
in vec4 FragPos;

uniform vec3 lightPos;

void main()
{
    // Manually sets depth map in the range [0, 1]
    gl_FragDepth = length(FragPos.xyz - lightPos) / 150.0f;
}
