#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"


#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "PositronEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"

#include <imgui/imgui.h>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

namespace PositronEngine
{
    GLfloat square_points_and_colors[] = {
        -1.0f, -1.0f, -1.0f,     1.0f, 0.5f, 0.0f,
        -1.0f, 1.0f, -1.0f,      0.5f, 1.0f, 0.5f,
        -1.0f, -1.0f, 1.0f,      1.0f, 1.0f, 0.5f,
        -1.0f, 1.0f, 1.0f,       0.5, 0.5f, 1.0f,

        1.0f, -1.0f, -1.0f,     1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,      0.5f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,      1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,       1.0, 0.0f, 1.0f


    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1,
        4, 5, 6, 7, 6, 5,
        0, 4, 6, 0, 2, 6,
        1, 5, 3, 3, 7, 5,
        3, 7, 2, 7, 6, 2,
        1, 5, 0, 5, 0, 4
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "uniform mat4 model_matrix;"
        "uniform mat4 view_projection_matrix;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    ShaderProgram* shader_program = nullptr;
    VertexBuffer* vertex_buffer_points_and_colors = nullptr;
    IndexBuffer* index_buffer = nullptr;

    VertexArray* vertex_array_object = nullptr;



    float location[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};

    float _background_color[4] = {0.0f, 0.0f, 0.0f, 0.0f };


    Application::Application()
    {
        LOG_INFORMATION("Stating application");
    }

    Application::~Application()
    {
        delete shader_program;
        delete vertex_buffer_points_and_colors;
        delete index_buffer;
        delete vertex_array_object;
        LOG_INFORMATION("Closing application");
    }

    int Application::start(unsigned int window_width,unsigned int window_height, const char* window_title)
    {
        _window = std::make_unique<Window>(window_title, window_width, window_height);

        _event_dispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFORMATION("Event MOUSE_MOVE is triggered. New mouse position is x:{0},y{1}", event.x, event.y);
            }
        );

        _event_dispatcher.addEventListener<EventWindowResized>(
            [](EventWindowResized& event)
            {
                LOG_INFORMATION("Event WINDOW_RESIZED is triggered. New window size is {0}x{1}", event.width, event.height);
            }
        );

        _event_dispatcher.addEventListener<EventWindowClose>(
            [&](EventWindowClose& event)
            {
                LOG_INFORMATION("Event WINDOW_CLOSING is triggered. Window is closing");
                _is_window_alive = false;

            }
        );

        _event_dispatcher.addEventListener<EventKeyPressed>(
            [](EventKeyPressed& event)
            {
                Input::pressKey(event.key_code);
                LOG_INFORMATION("Event KEYPRESSED : Key '{0} is pressed'",static_cast<char>(event.key_code));
            }
        );

        _event_dispatcher.addEventListener<EventKeyReleased>(
            [](EventKeyReleased& event)
            {
                Input::releaseKey(event.key_code);
                LOG_INFORMATION("Event KEYPRESSED : Key '{0} is released'",static_cast<char>(event.key_code));
            }
        );

        _window->setEventCallback(
            [&](BaseEvent& event)
            {
                _event_dispatcher.dispatch(event);
            }
        );

        /*убери потом это в объект сцены*/
        shader_program = new ShaderProgram(vertex_shader, fragment_shader);
        if(!shader_program->isCompile())
        {
            return -4;
        }


        BufferLayout buffer_layout_two_elements_vector3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };


        vertex_array_object = new VertexArray();
        vertex_buffer_points_and_colors = new VertexBuffer(square_points_and_colors,
                                                           sizeof(square_points_and_colors),
                                                           buffer_layout_two_elements_vector3);

        index_buffer = new IndexBuffer(indices, sizeof(indices) / sizeof(GLuint));

        vertex_array_object->addVertexBuffer(*vertex_buffer_points_and_colors);
        vertex_array_object->setIndexBuffer(*index_buffer);

        /*=========================================================*/

        while(_is_window_alive)
        {
            RenderOpenGL::setBackgroundColor(_background_color);
            RenderOpenGL::clear();

            shader_program->bind();

            glm::mat4 location_matrix(1,                    0,                  0,               0,
                                    0,                    1,                  0,               0,
                                    0,                    0,                  1,               0,
                                    location[0],          location[1],        location[2],     1);


            glm::mat4 rotate_matrix_x(1,    0,                                  0,                                  0,
                                    0,    cos(glm::radians(rotation[0])),     sin(glm::radians(rotation[0])),     0,
                                    0,    -sin(glm::radians(rotation[0])),    cos(glm::radians(rotation[0])),     0,
                                    0,    0,                                  0,                                  1);


            glm::mat4 rotate_matrix_y(cos(glm::radians(rotation[1])),       0,      -sin(glm::radians(rotation[1])),    0,
                                    0,                                    1,      0,                                  0,
                                    sin(glm::radians(rotation[1])),       0,      cos(glm::radians(rotation[1])),     0,
                                    0,                                    0,      0,                                  1);


            glm::mat4 rotate_matrix_z( cos(glm::radians(rotation[2])),      sin(glm::radians(rotation[2])),     0,      0,
                                    -sin(glm::radians(rotation[2])),       cos(glm::radians(rotation[2])),     0,      0,
                                    0,                                     0,                                  1,      0,
                                    0,                                     0,                                  0,      1);

            glm::mat4x4 scale_matrix = {scale[0], 0, 0, 0,
                                        0, scale[1], 0, 0,
                                        0, 0, scale[2], 0,
                                        0,  0,  0,  1};

            camera.setLocationAndRotation(glm::vec3(camera_location[0],camera_location[1],camera_location[2]),
                                        glm::vec3(camera_rotation[0],camera_rotation[1],camera_rotation[2]));

            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

            glm::mat4x4 model_matrix = location_matrix * rotate_matrix_x * rotate_matrix_y * rotate_matrix_z * scale_matrix;

            shader_program->setMatrix4("model_matrix", model_matrix);
            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());



            RenderOpenGL::draw(*vertex_array_object);

            GUImodule::onWindowStartUpdate();
            bool show = true;
            GUImodule::ShowExampleAppDockSpace(&show);

            ImGui::Begin("Color Picker");
            ImGui::SetWindowSize("Color Picker", ImVec2(350,60));
            ImGui::ColorEdit4("Color", _background_color);
            ImGui::End();

            ImGui::Begin("Local transform");
            ImGui::SetWindowSize("Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", location, -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", rotation, -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", scale, -2.0f, 2.0f);
            ImGui::End();

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();


            _window->onUpdate();
            onUpdate();
        }

        _window = nullptr;

        return 0;
    }
}
