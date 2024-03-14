#version 460

// uniforms
uniform mat4 model_matrix;
uniform mat4 view_projection_matrix;
uniform vec3 light_position;
uniform vec3 camera_position;

// vertex attribs (input)
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_tex_coord;

out DATA
{
    vec3 frag_normal;
    vec3 frag_position;
    vec2 texCoord;

    mat4 model;
    vec3 camera_position;
    vec3 light_position;

} data_out;

void main()
{
    gl_Position = view_projection_matrix * (model_matrix * vec4(vertex_position, 1.0));

    data_out.frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
    data_out.frag_position = (model_matrix * vec4(vertex_position, 1.0)).xyz;
    data_out.texCoord = vertex_tex_coord;

    data_out.model = model_matrix;
    data_out.camera_position = camera_position;
    data_out.light_position = light_position;

}
