#pragma once

struct GLFWwindow;

namespace PositronEngine
{
    class VertexArray;

    class RenderOpenGL
    {
        public:
            static bool initialize(GLFWwindow* window);

            static void setBackgroundColor(const float* background_color_array);
            static void clear();
            static void setViewport(const unsigned int width, const unsigned int height,
                                    const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

            static void enableDepth();
            static void disableDepth();

            static void draw(const VertexArray& vertex_array);

            static const char* getVersionOpenGL();
    };
}
