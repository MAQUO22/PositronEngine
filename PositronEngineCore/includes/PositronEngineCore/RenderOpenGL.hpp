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

            static void draw(VertexArray& vertex_array);

            static const char* getVersionOpenGL();

            static void setRunTime(const double new_time) { _run_time = new_time; }
            static double getRunTime() { return _run_time; }

            static void setElapsedTime(const double new_time) { _elapsed_time = new_time; }
            static double getElapsedTime() { return _elapsed_time; }

            static double getCurrentTime();

            static void enableSync();
            static void disableSync();

            static void enableFaceCulling();
            static void disableFaceCulling();

            static void postFrame(double& frame_time);

        private:
            static inline double _run_time {};
            static inline double _current_time {};
            static inline double _elapsed_time {};
            static inline int _frame_count {};
    };
}
