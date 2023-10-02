#pragma once

struct GLFWwindow;

namespace PositronEngine
{
    class GUImodule
    {
        public:
            static void onWindowCreate(GLFWwindow* window);
            static void onWindowClose();

            static void onWindowStartUpdate();
            static void onWindowUpdateDraw();

            static void ShowExampleAppDockSpace(bool* p_open);
    };
}
