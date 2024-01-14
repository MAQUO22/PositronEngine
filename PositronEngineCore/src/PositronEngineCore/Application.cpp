#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace PositronEngine
{
    bool show = true;
    int frame = 0;

    const char* frame_buffer_vertex = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;

    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        TexCoords = aTexCoords;
    }
    )";


    const char* frame_buffer_fragment = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D screenTexture;

    // Функция для вычисления гауссовой функции
    float gaussian(float x, float sigma)
    {
        return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * 3.14159) * sigma);
    }

    void main()
    {
        // Размеры экрана
        ivec2 screenSize = textureSize(screenTexture, 0);

        // Радиус размытия (можете настроить)
        float radius = 5.0;

        // Сумма весов для нормализации
        float weightSum = 0.0;

        vec3 finalColor = vec3(0.0);

        // Проход по соседним пикселям с использованием гауссового веса
        for (int i = -5; i <= 5; ++i)
        {
            for (int j = -5; j <= 5; ++j)
            {
                // Координаты текущего пикселя
                ivec2 currentCoords = ivec2(TexCoords * screenSize) + ivec2(i, j);

                // Нормализация координат
                vec2 texCoords = vec2(currentCoords) / vec2(screenSize);

                // Гауссов вес
                float weight = gaussian(length(vec2(i, j)), radius);

                // Суммирование взвешенных цветов
                finalColor += texture(screenTexture, texCoords).rgb * weight;

                // Сумма весов для нормализации
                weightSum += weight;
            }
        }

        // Нормализация и установка окончательного цвета
        FragColor = vec4(finalColor / weightSum, 1.0);
    }
)";



    GLuint fullscreenQuadVAO, fullscreenQuadVBO;

    // Данные для полноэкранного квада (прямоугольника)
    float quadVertices[] = {
        // Позиции           Текстурные координаты
        -1.0f,  1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f,        0.0f, 0.0f,
        1.0f,  1.0f,        1.0f, 1.0f,
        1.0f, -1.0f,        1.0f, 0.0f,
    };

    ShaderProgram* frame_buffer_program = nullptr;

    float glowRadius = 0.1f;
    int blurRadius = 15;

    Application::~Application()
    {
        LOG_INFORMATION("Closing application");
    }

    Application::Application()
    {
        LOG_INFORMATION("Stating application");
    }

    glm::vec2 Application::getCurrentCursorPosition() const
    {
        return _window->getCurrentCursorPosition();
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
                LOG_INFORMATION("Event KEY_PRESSED : Key '{0} is pressed'",static_cast<char>(event.key_code));
            }
        );

        _event_dispatcher.addEventListener<EventKeyReleased>(
            [](EventKeyReleased& event)
            {
                Input::releaseKey(event.key_code);
                LOG_INFORMATION("Event KEY_RELEASED : Key '{0} is released'",static_cast<char>(event.key_code));
            }
        );

        _event_dispatcher.addEventListener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event)
            {
                LOG_INFORMATION("Event MOUSE_BUTTON_PRESSED : Button '{0} is pressed (pos_x: {1}, pos_y: {2})'", static_cast<int>(event.mouse_button_code),
                event.x,
                event.y);

                Input::pressMouseButton(event.mouse_button_code);
                onMouseButtonEvent(event.mouse_button_code, event.x, event.y, true);
            }
        );

        _event_dispatcher.addEventListener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                LOG_INFORMATION("Event MOUSE_BUTTON_RELEASED : Button '{0} is released (pos_x: {1}, pos_y: {2})'", static_cast<int>(event.mouse_button_code),
                event.x,
                event.y);

                Input::releaseMouseButton(event.mouse_button_code);
                onMouseButtonEvent(event.mouse_button_code, event.x, event.y, false);
            }
        );

        _window->setEventCallback(
            [&](BaseEvent& event)
            {
                _event_dispatcher.dispatch(event);
            }
        );

        frame_buffer_program = new ShaderProgram(frame_buffer_vertex, frame_buffer_fragment);
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
        }

        compileShaders();
        initializeSpheres();


        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int texColorBuffer;
        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);


        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _window->getWidth(), _window->getHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete!");

        glGenVertexArrays(1, &fullscreenQuadVAO);
        glGenBuffers(1, &fullscreenQuadVBO);

        glBindVertexArray(fullscreenQuadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Позиции
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Текстурные координаты
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Очистка состояния
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        RenderOpenGL::setElapsedTime(0.0f);
        RenderOpenGL::setRunTime(0.0f);


        RenderOpenGL::enableSync();

        while(_is_window_alive)
        {

            double frame_time = RenderOpenGL::getCurrentTime() - RenderOpenGL::getRunTime();
            RenderOpenGL::setRunTime(RenderOpenGL::getCurrentTime());

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            RenderOpenGL::clear(); // буфер трафарета не используется
            RenderOpenGL::enableDepth();

            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

            onEditorUpdate();

            glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            frame_buffer_program->bind();
            frame_buffer_program->setVec2("lightPosition", glm::vec2(0.0f,0.0f));

            glBindVertexArray(fullscreenQuadVAO);
            RenderOpenGL::disableDepth();
            glBindTexture(GL_TEXTURE_2D, texColorBuffer);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);





            GUImodule::onWindowStartUpdate();
            GUImodule::ShowExampleAppDockSpace(&show);

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();



            frame++;
            RenderOpenGL::postFrame(frame_time);

            _window->onUpdate();
            onInputUpdate();

            GUImodule::onWindowStartUpdate();
            GUImodule::ShowExampleAppDockSpace(&show);
            onGUIdraw();
            GUImodule::onWindowUpdateDraw();

            frame++;
            RenderOpenGL::postFrame(frame_time);

            _window->onUpdate();
            onInputUpdate();


        }
        _window = nullptr;
        return 0;
    }
}
