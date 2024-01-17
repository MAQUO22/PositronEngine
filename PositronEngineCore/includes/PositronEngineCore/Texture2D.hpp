#pragma once
#include "PositronEngineCore/Bmp.h"

namespace PositronEngine
{
    class Texture2D
    {
        public:
            Texture2D(const Texture2D&) = delete;
            Texture2D& operator=(const Texture2D&) = delete;

            Texture2D(const char* path);
            Texture2D& operator=(Texture2D&& texture) noexcept;
            Texture2D(Texture2D&& texture) noexcept;

            ~Texture2D();

            void bind(const unsigned int unit) const;
            void unbind(const unsigned int unit) const;

        private:
            unsigned int _id = 0;
            Image::Bmp _image_bmp;
    };
}
