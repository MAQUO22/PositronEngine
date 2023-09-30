#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "PositronEngineCore/Rendering/OpenGL/IndexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace PositronEngine
{
    static bool is_GLFW_initialized_ = false;

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
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
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

    Window::Window(std::string title, unsigned int width, unsigned int height) :
        _window_data({std::move(title), width, height})
    {
        int result_code = initialization();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();

        ImGui_ImplGlfw_InitForOpenGL(_window, true);
    }

    Window::~Window()
    {
        shutDown();
    }

    void Window::onUpdate()
    {

        glClearColor(_background_color[0],_background_color[1],_background_color[2],_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiIO& input_output = ImGui::GetIO();
        input_output.DisplaySize.x = static_cast<float>(getWidth());
        input_output.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        ImGui::Begin("Color Picker");
        ImGui::SetWindowSize("Color Picker", ImVec2(300,60));
        ImGui::ColorEdit4("Color", _background_color);


        shader_program->bind();
        vertex_array_object->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array_object->getIndicesCount()), GL_UNSIGNED_INT, nullptr);


        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    int Window::initialization()
    {
        LOG_INFORMATION("Window '{0}' is created", _window_data._title);

        if(!is_GLFW_initialized_)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("GLFW has not been initialized!");
                return -1;
            }
            is_GLFW_initialized_ = true;
        }

        _window = glfwCreateWindow(_window_data._width, _window_data._heigth, _window_data._title.c_str(), nullptr, nullptr);
        if (!_window)
        {
            LOG_CRITICAL("Window '{0}' has not been created!", _window_data._title);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(_window);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Initialization of GLAD is failed");
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
                glViewport(0, 0, width, height);
            }
        );

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
        glfwDestroyWindow(_window);
        glfwTerminate();

        delete shader_program;
        delete vertex_buffer_points_and_colors;
        delete index_buffer;
        delete vertex_array_object;

        LOG_INFORMATION("Window '{0}' is terminated.", _window_data._title);
    }
}
