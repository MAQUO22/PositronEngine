#version 460

#define MAX_LIGHT_SOURCES 5

// uniforms
uniform mat4 model_matrix;
uniform mat4 view_projection_matrix;
uniform vec3 camera_position;

uniform vec3 light_direction;

uniform int number_of_point_lights;
uniform int number_of_spot_lights;

uniform vec3 point_light_positions[MAX_LIGHT_SOURCES];

uniform vec3 spot_light_positions[MAX_LIGHT_SOURCES];
uniform vec3 spot_light_direction[MAX_LIGHT_SOURCES];


// vertex attribs (input)
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_tex_coord;

out DATA
{
    vec3 frag_normal;
    vec2 tex_coord;

    mat4 view_projection_matrix;
    mat4 model_matrix;

    vec3 camera_position;

    vec3 light_direction;

    vec3 point_light_positions[MAX_LIGHT_SOURCES];

    vec3 spot_light_positions[MAX_LIGHT_SOURCES];
    vec3 spot_light_direction[MAX_LIGHT_SOURCES];
} data_out;

void main()
{
    gl_Position = model_matrix * vec4(vertex_position, 1.0);

    data_out.frag_normal = vertex_normal;
    data_out.tex_coord = vertex_tex_coord;

    data_out.model_matrix = model_matrix;
    data_out.view_projection_matrix = view_projection_matrix;

    data_out.camera_position = camera_position;
    data_out.light_direction = light_direction;

    for(int i = 0; i < number_of_point_lights; i++)
    {
        data_out.point_light_positions[i] = point_light_positions[i];
    }

    for(int j = 0; j < number_of_spot_lights; j++)
    {
        data_out.spot_light_positions[j] = spot_light_positions[j];
        data_out.spot_light_direction[j] = spot_light_direction[j];
    }

}
