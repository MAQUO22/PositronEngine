#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Camera.hpp"

#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "PositronEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp"

#include "PositronEngineCore/Modules/GUImodule.hpp"

#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>


#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace PositronEngine
{
    GLfloat square_points_and_colors[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.5f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0, 0.5f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
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

    Camera camera;

    float location[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};

    float camera_location[3] = {0.0f, 0.0f, 1.0f};
    float camera_rotation[3] = {0.0f, 0.0f, 0.0f};

    bool is_perspective_mode = false;

    Window::Window(std::string title, unsigned int width, unsigned int height) :
        _window_data({std::move(title), width, height})
    {
        int result_code = initialization();



    }

    Window::~Window()
    {
        shutDown();
    }

    void Window::onUpdate()
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

        ImGui::Begin("Camera transform");
        ImGui::SetWindowSize("Camera transform", ImVec2(400,100));
        ImGui::SliderFloat3("Location", camera_location, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotate", camera_rotation, -360.0f, 360.0f);
        ImGui::Checkbox("Perspective mode", &is_perspective_mode);
        ImGui::End();

        GUImodule::onWindowUpdateDraw();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    int Window::initialization()
    {
        LOG_INFORMATION("Window '{0}' is created", _window_data._title);

        if (!glfwInit())
        {
            LOG_CRITICAL("GLFW has not been initialized!");
            return -1;
        }

        _window = glfwCreateWindow(_window_data._width, _window_data._heigth, _window_data._title.c_str(), nullptr, nullptr);
        if (!_window)
        {
            LOG_CRITICAL("Window '{0}' has not been created!", _window_data._title);
            return -2;
        }

        if(!RenderOpenGL::initialize(_window))
        {
            return -3;
        }

        glfwSetWindowUserPointer(_window, &_window_data);

        glfwSetWindowSizeCallback(_window,
            [](GLFWwindow* window,int width, int height)
            {
                WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                window_data._width = width;
                window_data._heigth = height;

                EventWindowResized event(width, height);
                window_data._event_callback_function(event);
            });

        glfwSetCursorPosCallback(_window,
            [](GLFWwindow* window, double x, double y)
            {
                WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

                EventMouseMoved event(x,y);
                window_data._event_callback_function(event);
            }
        );

        glfwSetWindowCloseCallback(_window,
            [](GLFWwindow* window)
            {
                WindowData& window_data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

                EventWindowClose event;
                window_data._event_callback_function(event);
            }
        );

        glfwSetFramebufferSizeCallback(_window,
            [](GLFWwindow* window,int width, int height)
            {
                RenderOpenGL::setViewport(width, height);
            }
        );

        GUImodule::onWindowCreate(_window);

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

        return 0;
    }

    void Window::shutDown()
    {
        GUImodule::onWindowClose();
        glfwDestroyWindow(_window);
        glfwTerminate();

        delete shader_program;
        delete vertex_buffer_points_and_colors;
        delete index_buffer;
        delete vertex_array_object;

        LOG_INFORMATION("Window '{0}' is terminated.", _window_data._title);
    }
}
