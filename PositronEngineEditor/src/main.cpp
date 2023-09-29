#include<iostream>
#include<PositronEngineCore/Application.hpp>
#include<memory>

class EditorApplication : public PositronEngine::Application
{
    public:
        virtual void onUpdate() override
        {
            //std::cout << "Editor application working and showing frame number->" << std::endl;
        }

};

int main()
{
    auto application = std::make_unique<EditorApplication>();

    int editor_code = application->start(1280,720,"Positron Engine Editor");

    std::cin.get();
    return editor_code;
 }
