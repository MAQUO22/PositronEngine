#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform sampler2D bloomTexture;
uniform float gamma;
uniform float exposure;

void main()
{
    vec3 fragment = texture(image, TexCoords).rgb;
    vec3 bloom = texture(bloomTexture, TexCoords).rgb;

    vec3 color = fragment + bloom;

    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);
    FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
}
