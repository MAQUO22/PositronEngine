#include <PositronEngineCore/Application.hpp>
#include <PositronEngineCore/Input.hpp>

#include <memory>
#include <imgui/imgui.h>
#include <iostream>

class PositronEditor : public PositronEngine::Application
{
    public:
        virtual void onUpdate() override
        {
            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_W))
            {
                camera_location[2] -= camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_S))
            {
                camera_location[2] += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_D))
            {
                camera_location[0] += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_A))
            {
                camera_location[0] -= camera_speed;
            }

        }

        virtual void onGUIdraw() override
        {
            ImGui::Begin("Camera transform");
            ImGui::SetWindowSize("Camera transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", camera_location, -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", camera_rotation, -360.0f, 360.0f);
            ImGui::SliderFloat("Speed", &camera_speed, 0.0f, 0.1f);
            ImGui::Checkbox("Perspective camera mode", &is_perspective_mode);
            ImGui::End();
        }
};

int main()
{
    auto application = std::make_unique<PositronEditor>();

    int editor_code = application->start(1280,720,"Positron Engine Editor");

    std::cin.get();
    return editor_code;
 }
