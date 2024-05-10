#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/stb_image.h"
#include "PositronEngineCore/Mesh.hpp"

#include "PositronEngineCore/CubeMapTexture.hpp"
#include "PositronEngineCore/CubeMap.hpp"

#include "PositronEngineCore/FrameBuffer.hpp"
#include "PositronEngineCore/RenderBuffer.hpp"

#include "PositronEngineCore/Primitives/Model.hpp"
#include "PositronEngineCore/Primitives/CubePrimitive.hpp"
#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Primitives/PlatePrimitive.hpp"

#include "PositronEngineCore/LightSources/DirectionLight.hpp"
#include "PositronEngineCore/LightSources/PointLight.hpp"
#include "PositronEngineCore/LightSources/SpotLight.hpp"

#include <imgui/imgui.h>
#include <ImGuiFileDialog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace PositronEngine
{

    int max_light_sources_for_type = 4;
    bool cast_shadow = true;
    bool bloom_activate = false;
    bool draw_without_mesh = false;
    bool draw_skybox = true;

    bool is_primitive_changed = true;
    bool is_light_changed = true;

    float light_far_plane = 145.0f;

    float gamma = 0.280f;
    float exposure = 1.75f;

    bool show = true;
    int frame = 0;

    GLuint fullscreenQuadVAO, fullscreenQuadVBO;

    float quadVertices[] =
    {
       //Позиции         Текстурные координаты
        -1.0f,  1.0f,    0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f,
         1.0f, -1.0f,    1.0f, 0.0f,
    };

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

        camera.setViewportSize(static_cast<float>(window_width), static_cast<float>(window_height));

        _event_dispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFORMATION("Event MOUSE_MOVE is triggered. New mouse position is x:{0},y{1}", event.x, event.y);
            }
        );

        _event_dispatcher.addEventListener<EventWindowResized>(
            [&](EventWindowResized& event)
            {
                LOG_INFORMATION("Event WINDOW_RESIZED is triggered. New window size is {0}x{1}", event.width, event.height);
                camera.setViewportSize(event.width, event.height);
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

        std::shared_ptr<ShaderProgram> frame_buffer_program = std::make_shared<ShaderProgram>("post_processing.vert", "post_processing.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::shared_ptr<ShaderProgram> blur_program = std::make_shared<ShaderProgram>("post_processing.vert", "gaussian_blur.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("BLUR PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::shared_ptr<ShaderProgram> shadow_map_program = std::make_shared<ShaderProgram>("shadow_map.vert", "shadow_map.frag");
        if(!shadow_map_program->isCompile())
        {
            LOG_CRITICAL("SHADOW PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::shared_ptr<ShaderProgram> shadow_cube_map_program = std::make_shared<ShaderProgram>("shadow_cube_map.vert",
                                                                                                 "shadow_cube_map.frag",
                                                                                                 "shadow_cube_map.geom");
        if(!shadow_cube_map_program->isCompile())
        {
            LOG_CRITICAL("SHADOW CUBE PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::vector<Texture2D> textures_rock;
        textures_rock.push_back(Texture2D("rock_color.jpg", TextureType::diffuse));
        textures_rock.push_back(Texture2D("rock_roughness.jpg", TextureType::roughness));
        textures_rock.push_back(Texture2D("rock_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_brick;
        textures_brick.push_back(Texture2D("brick_color.jpg", TextureType::diffuse));
        textures_brick.push_back(Texture2D("brick_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_stone;
        textures_stone.push_back(Texture2D("rock2_color.jpg", TextureType::diffuse));
        textures_stone.push_back(Texture2D("rock2_roughness.jpg", TextureType::roughness));
        textures_stone.push_back(Texture2D("rock2_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_concrete;
        textures_concrete.push_back(Texture2D("concrete_color.jpg", TextureType::diffuse));
        textures_concrete.push_back(Texture2D("concrete_roughness.jpg", TextureType::roughness));
        textures_concrete.push_back(Texture2D("concrete_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_paving;
        textures_paving.push_back(Texture2D("paving_color.jpg", TextureType::diffuse));
        textures_paving.push_back(Texture2D("paving_roughness.jpg", TextureType::roughness));
        textures_paving.push_back(Texture2D("paving_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_light;

        std::shared_ptr<Material> rock = std::make_shared<Material>(textures_rock);
        std::shared_ptr<Material> brick = std::make_shared<Material>(textures_brick);
        std::shared_ptr<Material> stone = std::make_shared<Material>(textures_stone);
        std::shared_ptr<Material> concrete = std::make_shared<Material>(textures_concrete);
        std::shared_ptr<Material> paving = std::make_shared<Material>(textures_paving);

        std::shared_ptr<LightMaterial> light_material = std::make_shared<LightMaterial>(textures_light);


        _scene->addObject(std::make_unique<SpherePrimitive>("sphere"));
        _scene->getObjects()[0]->setLocation(0.0, 0.0, -2.0);
        _scene->getObjects()[0]->setScale(0.5, 0.5, 0.5);
        _scene->getObjects()[0]->setMaterial(brick);


        _scene->addObject(std::make_unique<CubePrimitive>("cube"));
        _scene->getObjects()[1]->setLocation(2.1, -2.1, 0.0);
        _scene->getObjects()[1]->setScale(0.5, 0.5, 0.5);
        _scene->getObjects()[1]->setMaterial(paving);

        _scene->addObject(std::make_unique<PlatePrimitive>("BOTTOM"));
        _scene->getObjects()[2]->setLocation(0.0, 0.0, -5.0);
        _scene->getObjects()[2]->setRotation(0.0, 90.0, 0.0);
        _scene->getObjects()[2]->setScale(1.0f, 10.0f, 10.0f);
        _scene->getObjects()[2]->setMaterial(concrete);


        _scene->addObject(std::make_unique<Model>("/home/n0rr/Desctop/C++/PositronEngine/ResourceFiles/Models/yorha_number_2_type_b/scene.gltf",
                                                  "2B"));
        _scene->addObject(std::make_unique<Model>("/home/n0rr/Desctop/C++/PositronEngine/ResourceFiles/Models/vr_room_-_art_gallery/scene.gltf",
                                                  "SCENE"));

        _scene->getObjects()[4]->setScale(2.0f, 2.0f, 2.0f);

        _scene->addLightObject(std::make_unique<DirectionLight>("dir"));
        _scene->addLightObject(std::make_unique<PointLight>("point"));
        _scene->addLightObject(std::make_unique<SpotLight>("spot"));

        for(int i = 0; i < _scene->getLightObjects().size(); i++)
        {
            if(_scene->getLightObjects()[i])
                _scene->getLightObjects()[i]->setLightMaterial(light_material);
        }

        _scene->removeObjectByName("cube");

        std::string facesCubemap[6] =
        {
            "../../ResourceFiles/Textures/SkyBox/Cloud/right.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/left.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/up.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/down.jpg",
            "../../ResourceFiles/Textures/SkyBox/Cloud/front.jpg",
            "../../ResourceFiles/Textures/SkyBox/Cloud/back.jpg"
        };

        CubeMapTexture cubemapTexture(facesCubemap);

        CubeMap skybox(cubemapTexture);

        FrameBuffer framebuffer;

        Texture2D post_processing_texture(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT0, post_processing_texture.getID());

        Texture2D bloomTexture(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT1, bloomTexture.getID());

        FrameBuffer::checkFrameBufferErrors();

        Texture2D image(_window->getWidth(), _window->getHeight(), TextureType::buffer);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        RenderBuffer render_buffer_scene(_window->getWidth(), _window->getHeight());

        FrameBuffer::checkFrameBufferErrors();


        FrameBuffer pingpongFBO;
        Texture2D pingpongBuffer(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        pingpongFBO.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer pingpongFBO1;
        Texture2D pingpongBuffer1(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        pingpongFBO1.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer1.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer viewport;

        Texture2D resultTextureID(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        viewport.connectTexture(GL_COLOR_ATTACHMENT0, resultTextureID.getID());

        FrameBuffer shadowMapFBO;

        Texture2D shadowMapDirectionTexture(4096, 4096, TextureType::shadow);
        shadowMapFBO.connectTexture(GL_DEPTH_ATTACHMENT, shadowMapDirectionTexture.getID());

        std::vector<FrameBuffer> spotShadowMapsFBO;
        std::vector<Texture2D> spotShadowsTextures;

        FrameBuffer::checkFrameBufferErrors();

        std::vector<FrameBuffer> shadowMapPointFBO;
        std::vector<CubeMapTexture> pointLightDepthCubeMap;

        FrameBuffer::checkFrameBufferErrors();

        glGenVertexArrays(1, &fullscreenQuadVAO);
        glGenBuffers(1, &fullscreenQuadVBO);
        glBindVertexArray(fullscreenQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        RenderOpenGL::setElapsedTime(0.0f);
        RenderOpenGL::setRunTime(0.0f);

        glEnable(GL_FRAMEBUFFER_SRGB);

        RenderOpenGL::disableFaceCulling();
        RenderOpenGL::enableSync();

        while(_is_window_alive)
        {
            //===============================================ПРЕД_КАДР========================================================================

            double frame_time = RenderOpenGL::getCurrentTime() - RenderOpenGL::getRunTime();
            RenderOpenGL::setRunTime(RenderOpenGL::getCurrentTime());


            //================================================ОБНОВЛЕНИЕ======================================================================

            _window->onUpdate();
            onInputUpdate();

            if(cast_shadow)
            {
                if(is_light_changed || is_primitive_changed)
                {
                    LOG_INFORMATION("CHANGED");
                    //===============================================SHADOW_MAP_DIRECTION_LIGH==========================================================


                    glEnable(GL_DEPTH_TEST);
                    glViewport(0, 0, shadowMapDirectionTexture.getWidth(), shadowMapDirectionTexture.getHeight());
                    shadowMapFBO.bind();
                    glClear(GL_DEPTH_BUFFER_BIT);

                    for(size_t i = 0; i < _scene->getObjects().size(); i++)
                    {
                        _scene->getObjects()[i]->draw(shadow_map_program, _scene->getLightObjects()[0]->getSpaceMatrix());
                    }

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);

                    //==============================================SHADOW_MAPS_POINT_LIGHT===========================================================

                    glEnable(GL_DEPTH_TEST);

                    shadowMapPointFBO.clear();
                    pointLightDepthCubeMap.clear();

                    for(size_t i = 0; i < LightTypeCounter::getNumberOfPointLights(); i++)
                    {
                        shadowMapPointFBO.emplace_back();
                        pointLightDepthCubeMap.emplace_back(1024, 1024);

                        shadowMapPointFBO[i].connectCubeMapTexture(GL_DEPTH_ATTACHMENT, pointLightDepthCubeMap[i].getID());
                    }

                    for(size_t j = 0, f = 0; j < _scene->getLightObjects().size(); j++)
                    {
                        if(_scene->getLightObjects()[j]->getLightType() == LightType::point)
                        {
                            glViewport(0, 0, 1024, 1024);
                            shadowMapPointFBO[f].bind();
                            glClear(GL_DEPTH_BUFFER_BIT);

                            shadow_cube_map_program->bind();
                            shadow_cube_map_program->setVec3("lightPos", _scene->getLightObjects()[j]->getLocationVec3());
                            shadow_cube_map_program->setFloat("farPlane", light_far_plane);

                            for(size_t k = 0; k < _scene->getObjects().size(); k++)
                            {
                                _scene->getObjects()[k]->draw(shadow_cube_map_program,
                                                              _scene->getLightObjects()[j]->getSpaceMatrices(light_far_plane));
                            }
                            f++;
                        }
                    }

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);

                    //==============================================SHADOW_MAPS_SPOT_LIGHT============================================================

                    glEnable(GL_DEPTH_TEST);

                    spotShadowMapsFBO.clear();
                    spotShadowsTextures.clear();

                    for(size_t i = 0; i < LightTypeCounter::getNumberOfSpotLights(); i++)
                    {
                        spotShadowMapsFBO.emplace_back();
                        spotShadowsTextures.emplace_back(1024, 1024, TextureType::shadow);

                        spotShadowMapsFBO[i].connectTexture(GL_DEPTH_ATTACHMENT, spotShadowsTextures[i].getID());
                    }

                    for(size_t j = 0, f = 0; j < _scene->getLightObjects().size(); j++)
                    {
                        if(_scene->getLightObjects()[j]->getLightType() == LightType::spot)
                        {
                            glViewport(0, 0, spotShadowsTextures[f].getWidth(), spotShadowsTextures[f].getHeight());
                            spotShadowMapsFBO[f].bind();
                            glClear(GL_DEPTH_BUFFER_BIT);

                            for(size_t k = 0; k < _scene->getObjects().size(); k++)
                            {
                                _scene->getObjects()[k]->draw(shadow_map_program,
                                                              _scene->getLightObjects()[j]->getSpaceMatrix());
                            }
                            f++;
                        }
                    }

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);

                    is_light_changed = false;
                    is_primitive_changed = false;
                }
            }

            //================================================БУФЕР_КАДРА_ПОСТ-ПРОЦЕССИНГА====================================================
            glViewport(0, 0, _window->getWidth(), _window->getHeight());

            framebuffer.bind();
            glClearColor(pow(0.0f, gamma),pow(0.0f, gamma), pow(0.0f, gamma), 1.0f);
            RenderOpenGL::clear();
            RenderOpenGL::enableDepth();

            if(draw_without_mesh)
            {
                for(size_t i = 0; i < _scene->getLightObjects().size(); i++)
                {
                    _scene->getLightObjects()[i]->drawWithoutMesh(camera);
                }
            }
            else
            {
                for(size_t i = 0; i < _scene->getLightObjects().size(); i++)
                {
                    _scene->getLightObjects()[i]->draw(camera);
                }
            }

            for(size_t i = 0; i < _scene->getObjects().size(); i++)
            {
                for(size_t j = 0, s = 0, p = 0; j < _scene->getLightObjects().size(); j++)
                {
                    if(_scene->getLightObjects()[j]->getLightType() == LightType::spot)
                    {
                        spotShadowsTextures[s].bindUnit(9 + s);
                        s++;
                    }
                    else if(_scene->getLightObjects()[j]->getLightType() == LightType::point)
                    {
                        pointLightDepthCubeMap[p].bindUnit(4 + p);
                        p++;
                    }
                }

                shadowMapDirectionTexture.bindUnit(3);

                _scene->getObjects()[i]->draw(camera, _scene->getLightObjects());
            }


            if(draw_skybox)
            {
                skybox.draw(camera);
            }

            //===============================================================================================================================

            //================================================БУФЕР_КАДРА_ДЛЯ_ПИНГ_ПОНГ_БЛЮРА================================================

            bool horizontal = true, first_iteration = true;
            int amount = 6;
            blur_program->bind();
            blur_program->setBool("bloom", bloom_activate);

            for(unsigned int i = 0; i < amount; i++)
            {
                horizontal ? pingpongFBO1.bind() : pingpongFBO.bind();
                blur_program->setBool("horizontal", horizontal);

                if(first_iteration)
                {
                    bloomTexture.bind();
                    first_iteration = false;
                }
                else
                {
                    horizontal ? pingpongBuffer.bind() : pingpongBuffer1.bind();
                }

                glBindVertexArray(fullscreenQuadVAO);
                RenderOpenGL::disableDepth();
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                horizontal = !horizontal;
            }


            //===============================================================================================================================


            //================================================БУФЕР_КАДРА_ДЛЯ_ВЬЮПОРТА=======================================================


            viewport.bind();
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            post_processing_texture.bindUnit(0);;
            pingpongBuffer.bindUnit(1);

            frame_buffer_program->bind();
            frame_buffer_program->setFloat("gamma", gamma);
            frame_buffer_program->setFloat("exposure", exposure);


            glBindVertexArray(fullscreenQuadVAO);
            RenderOpenGL::disableDepth();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            //===============================================================================================================================


            //===============================================ОСНОВНОЙ_БУФЕР_КАДРА============================================================

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //===============================================ПОЛЬЗОВАТЕЛЬСКИЙ+ИНТЕРФЕЙС======================================================

            GUImodule::onWindowStartUpdate();
            GUImodule::ShowExampleAppDockSpace(&show);

            ImGui::Begin("Scene");

            ImGui::Text("SCENE NAME - ");
            ImGui::SameLine();
            ImGui::Text("%s", _scene->getName().c_str());
            ImGui::Spacing();

            static char objectNameBuffer[30] = "";

            ImGui::Text("Enter name :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(135);
            ImGui::InputText("##InputField", objectNameBuffer, sizeof(objectNameBuffer));

            ImGui::Text("Objects: ");
            if (ImGui::Button("Add Cube")) {
                _scene->addObject(std::make_unique<CubePrimitive>(std::string(objectNameBuffer)));
                _scene->getObjects().back()->setMaterial(stone);

                is_primitive_changed = true;
                objectNameBuffer[0] = '\0';
            }

            ImGui::SameLine();
            if(ImGui::Button("Add Sphere"))
            {
                _scene->addObject(std::make_unique<SpherePrimitive>(std::string(objectNameBuffer)));
                _scene->getObjects().back()->setMaterial(stone);

                is_primitive_changed = true;
                objectNameBuffer[0] = '\0';
            }

            ImGui::SameLine();
            if(ImGui::Button("Add Plate"))
            {
                _scene->addObject(std::make_unique<PlatePrimitive>(std::string(objectNameBuffer)));
                _scene->getObjects().back()->setMaterial(stone);

                is_primitive_changed = true;
                objectNameBuffer[0] = '\0';
            }

            if (ImGui::Button("Add Model"))
            {
                IGFD::FileDialogConfig config;
                config.path = "../../ResourceFiles/Models/";
                config.sidePaneWidth = 800; // Ширина окна
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*,.obj,.fbx,.gltf", config);
            }

            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

                    _scene->addObject(std::make_unique<Model>(filePathName.c_str(), std::string(objectNameBuffer)));
                }

                ImGuiFileDialog::Instance()->Close();

                is_primitive_changed = true;
                objectNameBuffer[0] = '\0';
            }

            ImGui::Spacing();
            ImGui::Text("Light Objects: ");

            if(ImGui::Button("Add Point Light"))
            {
                if(LightTypeCounter::getNumberOfPointLights() < max_light_sources_for_type)
                {
                    _scene->addLightObject(std::make_unique<PointLight>(std::string(objectNameBuffer)));
                    _scene->getLightObjects().back()->setLightMaterial(light_material);

                    is_light_changed = true;
                    objectNameBuffer[0] = '\0';
                }
            }
            ImGui::SameLine();

            if(ImGui::Button("Add Spot Light"))
            {
                if(LightTypeCounter::getNumberOfSpotLights() < max_light_sources_for_type)
                {
                    _scene->addLightObject(std::make_unique<SpotLight>(std::string(objectNameBuffer)));
                    _scene->getLightObjects().back()->setLightMaterial(light_material);

                    is_light_changed = true;
                    objectNameBuffer[0] = '\0';
                }
            }

            ImGui::Spacing();

            if (ImGui::TreeNode("Root")) {
                if (ImGui::TreeNode("Objects")) {
                    for(size_t i = 0; i < _scene->getObjects().size(); i++)
                    {
                        if (ImGui::Selectable(_scene->getObjects()[i]->getName().c_str(), _scene->getSelectedObjectIndex() == i))
                        {
                            _scene->setSelectedObjectIndex(i);
                            _scene->setSelectedLightObjectIndex(-1);
                        }

                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Light Objects")) {
                    for(size_t l = 0; l < _scene->getLightObjects().size(); l++)
                    {
                        if (ImGui::Selectable(_scene->getLightObjects()[l]->getName().c_str(), _scene->getSelectedLightObjectIndex() == l))
                        {
                            _scene->setSelectedObjectIndex(-1);
                            _scene->setSelectedLightObjectIndex(l);
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::End();

            ImGui::Begin("Viewport");
            ImVec2 viewport_size = ImGui::GetWindowSize();
            camera.setViewportSize(viewport_size.x, viewport_size.y);
            ImGui::Image((void*)(intptr_t)resultTextureID.getID(), viewport_size);
            ImGui::End();

            ImGui::Begin("Post-processing");
            ImGui::Checkbox("Bloom activate", &bloom_activate);
            ImGui::Checkbox("Draw SkyBox", &draw_skybox);
            ImGui::SliderFloat("Gamma", &gamma, 0.0f, 3.0f);
            ImGui::SliderFloat("Exposure", &exposure, 0.1f, 2.0f);
            ImGui::End();

            ImGui::Begin("Properties");
            if(_scene->getSelectedObject())
            {
                if(_scene->getObjects().size() > 0)
                {
                    ImGui::Text("OBJECT - ");
                    ImGui::SameLine();
                    ImGui::Text("%s", _scene->getSelectedObject()->getName().c_str());
                    ImGui::SameLine();

                    if(ImGui::Button("DELETE"))
                    {
                        _scene->removeObjectByName(_scene->getSelectedObject()->getName());
                        is_primitive_changed = true;

                    }
                }

                if(_scene->getSelectedObject())
                {
                    ImGui::Spacing();
                    ImGui::Text("LOCATION:");

                    ImGui::Text("X:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##XSlider", &_scene->getSelectedObject()->getLocation()[0], -25.0f, 25.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Y:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##YSlider", &_scene->getSelectedObject()->getLocation()[1], -25.0f, 25.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Z:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##ZSlider", &_scene->getSelectedObject()->getLocation()[2], -25.0f, 25.0f))
                        is_primitive_changed = true;

                    ImGui::Spacing();
                    ImGui::Text("ROTATION:");

                    ImGui::Text("X:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##XRSlider", &_scene->getSelectedObject()->getRotation()[0], -360.0f, 360.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Y:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##YRSlider", &_scene->getSelectedObject()->getRotation()[1], -360.0f, 360.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Z:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##ZRSlider", &_scene->getSelectedObject()->getRotation()[2], -360.0f, 360.0f))
                        is_primitive_changed = true;

                    ImGui::Spacing();
                    ImGui::Text("SCALE:");

                    ImGui::Text("X:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##XSSlider", &_scene->getSelectedObject()->getScale()[0], -3.0f, 3.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Y:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##YSSlider", &_scene->getSelectedObject()->getScale()[1], -3.0f, 3.0f))
                        is_primitive_changed = true;

                    ImGui::SameLine();
                    ImGui::Text("Z:");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    if (ImGui::SliderFloat("##ZSSlider", &_scene->getSelectedObject()->getScale()[2], -3.0f, 3.0f))
                        is_primitive_changed = true;

                    ImGui::Spacing();
                    ImGui::Text("MATERIAL:");
                    if(ImGui::SliderFloat("ambient_factor", _scene->getSelectedObject()->getMaterial()->getAmbientFactor(), 0.0f, 2.0f))
                        _scene->getSelectedObject()->getMaterial()->setAmbientFactor(*_scene->getSelectedObject()->getMaterial()->getAmbientFactor());

                    if(ImGui::SliderFloat("diffuse_factor", _scene->getSelectedObject()->getMaterial()->getDiffuseFactor(), 0.0f, 2.0f))
                        _scene->getSelectedObject()->getMaterial()->setDiffuseFactor(*_scene->getSelectedObject()->getMaterial()->getDiffuseFactor());

                    if(!_scene->getSelectedObject()->getMaterial()->checkRoughnessMap())
                        if(ImGui::SliderFloat("roughness", _scene->getSelectedObject()->getMaterial()->getRoughnessFactor(), 1.0f, 128.0f))
                            _scene->getSelectedObject()->getMaterial()->setRoughnessFactor(*_scene->getSelectedObject()->getMaterial()->getRoughnessFactor());

                    if(ImGui::SliderFloat("specular_factor", _scene->getSelectedObject()->getMaterial()->getSpecularFactor(), 0.0f, 1.0f))
                        _scene->getSelectedObject()->getMaterial()->setSpecularFactor(*_scene->getSelectedObject()->getMaterial()->getSpecularFactor());

                    if(ImGui::SliderFloat("metallic_factor", _scene->getSelectedObject()->getMaterial()->getMetallicFactor(), 0.0f, 1.0f))
                        _scene->getSelectedObject()->getMaterial()->setMetallicFactor(*_scene->getSelectedObject()->getMaterial()->getMetallicFactor());
                }
            }
            else if (_scene->getSelectedLightObject())
            {
                if(_scene->getLightObjects().size() > 0)
                {
                    ImGui::Text("LIGHT OBJECT - ");
                    ImGui::SameLine();
                    ImGui::Text("%s", _scene->getSelectedLightObject()->getName().c_str());
                    ImGui::SameLine();

                    if(_scene->getSelectedLightObject()->getLightType() != LightType::direction)
                    {
                        if(ImGui::Button("DELETE"))
                        {
                            _scene->removeLightObjectByName(_scene->getSelectedLightObject()->getName());
                            is_light_changed = true;
                        }
                    }

                    ImGui::Spacing();

                    if(_scene->getSelectedLightObject())
                    {
                        ImGui::Spacing();
                        ImGui::Text("LIGHT COLOR: ");
                        ImGui::ColorEdit3("color", _scene->getSelectedLightObject()->getColor());

                        if(_scene->getSelectedLightObject()->getLightType() != LightType::direction)
                        {
                            ImGui::Spacing();
                            ImGui::Text("LOCATION:");
                            ImGui::Text("X:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##XSlider", &_scene->getSelectedLightObject()->getLocation()[0], -25.0f, 25.0f))
                                is_light_changed = true;

                            ImGui::SameLine();
                            ImGui::Text("Y:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##YSlider", &_scene->getSelectedLightObject()->getLocation()[1], -25.0f, 25.0f))
                                is_light_changed = true;

                            ImGui::SameLine();
                            ImGui::Text("Z:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##ZSlider", &_scene->getSelectedLightObject()->getLocation()[2], -25.0f, 25.0f))
                                is_light_changed = true;
                        }

                        if(_scene->getSelectedLightObject()->getLightType() != LightType::point)
                        {
                            ImGui::Spacing();
                            ImGui::Text("DIRECTION:");

                            ImGui::Text("X:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##XDSlider", &_scene->getSelectedLightObject()->getDirection()[0], -20.0f, 20.0f))
                                is_light_changed = true;

                            ImGui::SameLine();
                            ImGui::Text("Y:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##YDSlider", &_scene->getSelectedLightObject()->getDirection()[1], -20.0f, 20.0f))
                                is_light_changed = true;

                            ImGui::SameLine();
                            ImGui::Text("Z:");
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(120);
                            if (ImGui::SliderFloat("##ZDSlider", &_scene->getSelectedLightObject()->getDirection()[2], -20.0f, 20.0f))
                                is_light_changed = true;
                        }

                        if(_scene->getSelectedLightObject()->getLightType() == LightType::point)
                        {
                            ImGui::Spacing();
                            ImGui::Text("ADVANCED: ");

                            if(ImGui::SliderFloat("linear_attenation",
                                _scene->getSelectedLightObject()->getPtrLinearAttenuation() ,0.0f, 0.3f))
                                is_light_changed = true;

                            if(ImGui::SliderFloat("constant_attenation",
                                _scene->getSelectedLightObject()->getPtrConstantAttenuation() ,0.0f, 0.3f))
                                is_light_changed = true;
                        }

                        if(_scene->getSelectedLightObject()->getLightType() == LightType::spot)
                        {
                            ImGui::Spacing();
                            ImGui::Text("ADVANCED: ");

                            if(ImGui::SliderFloat("Inner_cone", _scene->getSelectedLightObject()->getPtrInnerCone(),
                                _scene->getSelectedLightObject()->getOuterCone(), 2.0f))
                                is_light_changed = true;

                            if(ImGui::SliderFloat("Outer_cone", _scene->getSelectedLightObject()->getPtrOuterCone(), 0.0f,
                                _scene->getSelectedLightObject()->getInnerCone()))
                                is_light_changed = true;
                        }
                    }
                }
            }

            ImGui::End();

            onGUIdraw();
            GUImodule::onWindowUpdateDraw();

            //===============================================================================================================================

            //===============================================ПОСТ_КАДР=======================================================================
            frame++;
            RenderOpenGL::postFrame(frame_time);
        }
        _window = nullptr;
        return 0;
    }
}
