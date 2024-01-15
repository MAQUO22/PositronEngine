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
    float gamma = 0.280f;
    float exposure = 1.75f;
    bool show = true;
    int frame = 0;


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

        frame_buffer_program = new ShaderProgram("post_processing.vert", "post_processing.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        compileShaders();
        initializeSpheres();


        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int texColorBuffer;
        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        unsigned int bloomTexture;
        glGenTextures(1, &bloomTexture);
        glBindTexture(GL_TEXTURE_2D, bloomTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);


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

        glEnable(GL_FRAMEBUFFER_SRGB);

        RenderOpenGL::enableSync();

        while(_is_window_alive)
        {
            double frame_time = RenderOpenGL::getCurrentTime() - RenderOpenGL::getRunTime();
            RenderOpenGL::setRunTime(RenderOpenGL::getCurrentTime());

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(pow(0.0f, gamma),pow(0.0f, gamma), pow(0.0f, gamma), 1.0f);
            RenderOpenGL::clear();
            RenderOpenGL::enableDepth();

            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

            onEditorUpdate();

            GUImodule::onWindowStartUpdate();
            GUImodule::ShowExampleAppDockSpace(&show);

            ImGui::Begin("Post-processing");
            ImGui::SliderFloat("Gamma", &gamma, 0.0f, 3.0f);
            ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.0f);
            ImGui::End();

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            frame_buffer_program->bind();
            frame_buffer_program->setFloat("gamma", gamma);
            frame_buffer_program->setFloat("exposure", exposure);


            glBindVertexArray(fullscreenQuadVAO);
            RenderOpenGL::disableDepth();
            glBindTexture(GL_TEXTURE_2D, texColorBuffer);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            _window->onUpdate();
            onInputUpdate();

            frame++;
            RenderOpenGL::postFrame(frame_time);
        }
        _window = nullptr;
        return 0;
    }
}
