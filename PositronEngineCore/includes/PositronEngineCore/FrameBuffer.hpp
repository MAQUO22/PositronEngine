#pragma once

namespace PositronEngine
{
    class FrameBuffer
    {
        public:
            FrameBuffer();

            ~FrameBuffer();

            void bind();
            void connectTexture(unsigned int attachment, unsigned int id);
            static void checkFrameBufferErrors();

        private:
            unsigned int _id = 0;
    };
}
