#version 460

// uniforms
uniform mat4 model_matrix;
uniform mat4 view_projection_matrix;

// vertex attribs (input)
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_tex_coord;

// varyings (output)
out vec3 frag_normal;
out vec3 frag_position;
out vec2 texCoordGround;

void main()
{
    frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
    vec4 world_vertex_position = model_matrix * vec4(vertex_position, 1.0);
    frag_position = world_vertex_position.xyz;
    texCoordGround = vertex_tex_coord;

    gl_Position = view_projection_matrix * world_vertex_position;
}
