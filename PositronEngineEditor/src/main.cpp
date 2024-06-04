#include <PositronEngineCore/Application.hpp>
#include <PositronEngineCore/Input.hpp>
#include <PositronEngineCore/Scene.hpp>

#include <PositronEngineCore/Primitives/SpherePrimitive.hpp>
#include <PositronEngineCore/Primitives/CubePrimitive.hpp>
#include <PositronEngineCore/Primitives/PlatePrimitive.hpp>
#include <PositronEngineCore/LightSources/DirectionLight.hpp>
#include <PositronEngineCore/Primitives/Model.hpp>

#include <memory>
#include <imgui/imgui.h>
#include <iostream>
#include <backends/imgui_impl_opengl3.h>


class PositronEditor : public PositronEngine::Application
{
    public:
        double initial_mouse_position_x = 0.0f;
        double initial_mouse_position_y = 0.0f;

        ~PositronEditor()
        {
        }

        virtual void onInputUpdate() override
        {

            glm::vec3 movement_delta{0, 0, 0};
            glm::vec3 rotation_delta{0, 0, 0};

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_W))
            {
                movement_delta.x += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_S))
            {
                movement_delta.x -= camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_D))
            {
                movement_delta.y += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_A))
            {
                movement_delta.y -= camera_speed;
            }


            if(PositronEngine::Input::isMouseButtonPressed(PositronEngine::MouseButtonCode::MOUSE_BUTTON_RIGHT))
            {
                glm::vec2 current_cursor_position = getCurrentCursorPosition();

                if(PositronEngine::Input::isMouseButtonPressed(PositronEngine::MouseButtonCode::MOUSE_BUTTON_LEFT))
                {
                    camera.moveRight(static_cast<float>(current_cursor_position.x - initial_mouse_position_x) / 100.0f);
                    camera.moveUp(static_cast<float>(current_cursor_position.y - initial_mouse_position_y) / 100.0f);
                }
                else
                {
                    rotation_delta.z += static_cast<float>(current_cursor_position.x - initial_mouse_position_x) / 5.0f;
                    rotation_delta.y -= static_cast<float>(initial_mouse_position_y - current_cursor_position.y) / 5.0f;
                }

                initial_mouse_position_x = current_cursor_position.x;
                initial_mouse_position_y = current_cursor_position.y;
            }

            camera.moveAndRotation(movement_delta, rotation_delta);
        }


        void onMouseButtonEvent(const PositronEngine::MouseButtonCode mouse_button, const double x, const double y, bool pressed) override
        {
            initial_mouse_position_x = x;
            initial_mouse_position_y = y;
        }

        virtual void onGUIdraw() override
        {
            camera_location[0] = camera.getLocation().x;
            camera_location[1] = camera.getLocation().y;
            camera_location[2] = camera.getLocation().z;

            camera_rotation[0] = camera.getRotation().x;
            camera_rotation[1] = camera.getRotation().y;
            camera_rotation[2] = camera.getRotation().z;

            camera_FOV = camera.getFieldOfView();
            camera_far_plane = camera.getFarPlane();
            camera_near_plane = camera.getNearPlane();

            ImGui::Begin("Camera transform");
            ImGui::SetWindowSize("Camera transform", ImVec2(400,100));
            if(ImGui::SliderFloat3("Location", camera_location, -10.0f, 10.0f))
            {
                camera.setLocation(glm::vec3(camera_location[0], camera_location[1], camera_location[2]));
            }
            if(ImGui::SliderFloat3("Rotate", camera_rotation, -360.0f, 360.0f))
            {
                camera.setRotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
            }
            ImGui::SliderFloat("Speed", &camera_speed, 0.05f, 0.50f);

            if(ImGui::SliderFloat("Camera FOV", &camera_FOV, 45.0f, 120.0f))
            {
                camera.setFieldOfView(camera_FOV);
            }

            if(ImGui::SliderFloat("Camera near plane", &camera_near_plane, 0.1f, 10.0f))
            {
                camera.setNearPlane(camera_near_plane);
            }

            if(ImGui::SliderFloat("Camera far plane", &camera_far_plane, 1.0f, 800.0f))
            {
                camera.setFarPlane(camera_far_plane);
            }

            if(ImGui::Checkbox("Perspective camera mode", &is_perspective_mode))
            {
                camera.setProjection(is_perspective_mode ? PositronEngine::Camera::ProjectionMode::Perspective :                PositronEngine::Camera::ProjectionMode::Orthographic);
            }

            ImGui::End();
        }
};

int main()
{
    auto application = std::make_unique<PositronEditor>();
    auto scene = std::make_shared<PositronEngine::Scene>("scene1");

    application->setScene(scene); // Передача объекта сцены в приложение
    application->start(1280, 720, "Positron Engine Editor");
    std::cin.get();
    return 0;

 }
