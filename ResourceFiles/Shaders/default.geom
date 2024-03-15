#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 tex_coord;
out vec3 camera_position;
out vec3 light_position;

in DATA
{
    vec3 frag_normal;
    vec2 tex_coord;

    mat4 view_projection_matrix;
    mat4 model_matrix;

    vec3 camera_position;
    vec3 light_position;
} data_in[];

void main()
{
    vec4 vertex0 = data_in[0].model_matrix * vec4(gl_in[0].gl_Position.xyz, 1.0);
    vec4 vertex1 = data_in[1].model_matrix * vec4(gl_in[1].gl_Position.xyz, 1.0);
    vec4 vertex2 = data_in[2].model_matrix * vec4(gl_in[2].gl_Position.xyz, 1.0);

    vec3 edge0 = vertex1.xyz - vertex0.xyz;
    vec3 edge1 = vertex2.xyz - vertex0.xyz;

    vec2 delta_UV0 = data_in[1].tex_coord - data_in[0].tex_coord;
    vec2 delta_UV1 = data_in[2].tex_coord - data_in[0].tex_coord;

    float inv_det = 1.0f / (delta_UV0.x * delta_UV1.y - delta_UV1.x * delta_UV0.y);

    vec3 tangent = vec3(inv_det * (delta_UV1.y * edge0 - delta_UV0.y * edge1));
    vec3 bitangent = vec3(inv_det * (-delta_UV1.x * edge0 + delta_UV0.x * edge1));

    vec3 T = normalize(vec3(data_in[0].model_matrix * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(data_in[0].model_matrix * vec4(bitangent, 0.0f)));

    for(int i = 0; i < 3; i++)
    {
        gl_Position = data_in[i].view_projection_matrix * gl_in[i].gl_Position;

        vec3 N = normalize(vec3(data_in[i].model_matrix * vec4(data_in[i].frag_normal, 0.0f)));

        mat3 TBN = mat3(T, B, N);
        TBN = transpose(TBN);

        frag_normal = data_in[i].frag_normal;
        tex_coord = data_in[i].tex_coord;

        frag_position = TBN * gl_in[i].gl_Position.xyz;
        camera_position = TBN * data_in[i].camera_position;
        light_position = TBN * data_in[i].light_position;

        EmitVertex();
    }

    EndPrimitive();
}
