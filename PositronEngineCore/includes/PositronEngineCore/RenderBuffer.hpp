#pragma once

namespace PositronEngine
{
    class RenderBuffer
    {
        public:
            RenderBuffer(const unsigned int size_x, const unsigned int size_y);

            ~RenderBuffer();

            void bind();

        private:
            unsigned int _id = 0;
    };
}

