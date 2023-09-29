#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace PositronEngine
{
    static bool is_GLFW_initialized_ = false;

    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    GLfloat points_and_colors[] = {
        0.0f, 0.5f, 0.0f,   1.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 1.0f
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

    //УМНЫЕ УКАЗАТЕЛИ ВЫЗЫВАЮТ ОШИБКУ СЕГМЕНТИРОВАНИЯ ПРИ УДАЛЕНИИ
    //std::unique_ptr<ShaderProgram> shader_program = nullptr;

    //std::unique_ptr<VertexBuffer> vertex_buffer_points = nullptr;
    //std::unique_ptr<VertexBuffer> vertex_buffer_colors = nullptr;
    //std::unique_ptr<VertexBuffer> vertex_buffer_points_and_colors = nullptr;

    //std::unique_ptr<VertexArray> vertex_array_object_one_buffer = nullptr;
    //std::unique_ptr<VertexArray> vertex_array_object_two_buffers = nullptr;

    ShaderProgram* shader_program = nullptr;
    VertexBuffer* vertex_buffer_points = nullptr;
    VertexBuffer* vertex_buffer_colors = nullptr;
    VertexBuffer* vertex_buffer_points_and_colors = nullptr;

    VertexArray* vertex_array_object_one_buffer = nullptr;
    VertexArray* vertex_array_object_two_buffers = nullptr;

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
        ImGui::SetWindowSize("Color Picker", ImVec2(300,90));
        ImGui::ColorEdit4("Color", _background_color);

        static bool use_buffer_with_two_objects = true;
        ImGui::Checkbox("use buffer with 2 objects", &use_buffer_with_two_objects);

        shader_program->bind();

        if(use_buffer_with_two_objects)
        {
            vertex_array_object_two_buffers->bind();
        }
        else{
            vertex_array_object_one_buffer->bind();
        }


        glDrawArrays(GL_TRIANGLES, 0, 3);


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

        //shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        shader_program = new ShaderProgram(vertex_shader, fragment_shader);
        if(!shader_program->isCompile())
        {
            return -4;
        }

        BufferLayout buffer_layout_one_element_vector3
        {
            ShaderDataType::Float3
        };

        //vertex_array_object_two_buffers = std::make_unique<VertexArray>();

        //vertex_buffer_points = std::make_unique<VertexBuffer>(points, sizeof(points), buffer_layout_one_element_vector3);
        //vertex_buffer_colors = std::make_unique<VertexBuffer>(colors, sizeof(colors), buffer_layout_one_element_vector3);

        //vertex_array_object_two_buffers->addBuffer(*vertex_buffer_points);
        //vertex_array_object_two_buffers->addBuffer(*vertex_buffer_colors);

        vertex_array_object_two_buffers = new VertexArray();

        vertex_buffer_points = new VertexBuffer(points, sizeof(points), buffer_layout_one_element_vector3);
        vertex_buffer_colors = new VertexBuffer(colors, sizeof(colors), buffer_layout_one_element_vector3);

        vertex_array_object_two_buffers->addBuffer(*vertex_buffer_points);
        vertex_array_object_two_buffers->addBuffer(*vertex_buffer_colors);



        BufferLayout buffer_layout_two_elements_vector3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        //vertex_array_object_one_buffer = std::make_unique<VertexArray>();
        //vertex_buffer_points_and_colors = std::make_unique<VertexBuffer>(points_and_colors,
        //                                                                 sizeof(points_and_colors),
        //                                                                 buffer_layout_two_elements_vector3);

        //vertex_array_object_one_buffer->addBuffer(*vertex_buffer_points_and_colors);

        vertex_array_object_one_buffer = new VertexArray();
        vertex_buffer_points_and_colors = new VertexBuffer(points_and_colors,
                                                                         sizeof(points_and_colors),
                                                                         buffer_layout_two_elements_vector3);

        vertex_array_object_one_buffer->addBuffer(*vertex_buffer_points_and_colors);

        return 0;
    }

    void Window::shutDown()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();

        delete shader_program;
        delete vertex_buffer_points;
        delete vertex_buffer_colors;
        delete vertex_buffer_points_and_colors;
        delete vertex_array_object_one_buffer;
        delete vertex_array_object_two_buffers;

        LOG_INFORMATION("Window '{0}' is terminated.", _window_data._title);
    }
}
