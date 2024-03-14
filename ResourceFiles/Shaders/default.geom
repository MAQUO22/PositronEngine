#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 texCoord;
out vec3 camera_position;
out vec3 light_position;

in DATA
{
    vec3 frag_normal;
    vec2 texCoord;

    mat4 model;
    vec3 camera_position;
    vec3 light_position;
} data_in[];


// Default main function
void main()
{
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;


    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(data_in[0].model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(data_in[0].model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(data_in[0].model * vec4(cross(edge1, edge0), 0.0f)));

    mat3 TBN = mat3(T, B, N);

    TBN = transpose(TBN);

    for(int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;

        frag_normal = data_in[i].frag_normal;
        texCoord = data_in[i].texCoord;

        frag_position = TBN * gl_in[i].gl_Position.xyz;
        camera_position = TBN * data_in[i].camera_position;
        light_position = TBN * data_in[i].light_position;

        EmitVertex();
    }

    EndPrimitive();
}
