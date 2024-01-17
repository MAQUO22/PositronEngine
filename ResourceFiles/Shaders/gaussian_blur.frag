#version 330 core

out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D screen_texture;
uniform bool horizontal;
//uniform float weight[5] = float[] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

uniform float weight[9] = float[] (0.05f, 0.09f, 0.12f, 0.15f, 0.18f, 0.15f, 0.12f, 0.09f, 0.05f);

void main()
{
    vec2 tex_offset = 1.0f / textureSize(screen_texture, 0);
    vec3 result = texture(screen_texture, TexCoords).rgb * weight[0];

    if(horizontal)
    {
        for(int i = 1; i < 9; i++)
        {
            result += texture(screen_texture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screen_texture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 9; i++)
        {
            result += texture(screen_texture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(screen_texture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    frag_color = vec4(result, 1.0f);
}
