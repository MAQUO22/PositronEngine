#version 460 core

out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D screen_texture;
uniform bool horizontal;
uniform bool bloom;
uniform float weight[5] = float[](0.05, 0.15, 0.3, 0.15, 0.05);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(screen_texture, 0);
    vec3 result = texture(screen_texture, TexCoords).rgb * weight[0];

    if(bloom)
    {
        if(horizontal)
        {
            for(int i = 1; i < 7; i++)
            {
                result += texture(screen_texture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i % 5];
                result += texture(screen_texture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i% 5];
            }
        }
        else
        {
            for(int i = 1; i < 7; i++)
            {
                result += texture(screen_texture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i% 5];
                result += texture(screen_texture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i% 5];
            }
        }
    }

    frag_color = vec4(result, 1.0);
}









