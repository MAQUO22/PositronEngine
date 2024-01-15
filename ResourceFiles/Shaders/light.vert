#version 460

// uniforms
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform mat4 view_projection_matrix;

// vertex attribs (input)
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_tex_coord;

// varyings (output)
out vec3 esNormal;
out vec3 color;
out vec2 texCoord;

void main()
{
    esNormal = vec3(normal_matrix * vec4(vertex_normal, 1.0));
    texCoord = vertex_tex_coord;

    gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
}
