#pragma once
#include "PositronEngineCore/stb_image.h"

namespace PositronEngine
{
    enum class TextureType
    {
        diffuse,
        specular,
        normal
    };

    class Texture2D
    {
        public:
            Texture2D(Texture2D&);
            Texture2D& operator=(const Texture2D&) = delete;

            Texture2D(const unsigned int size_x, const unsigned int size_y);
            Texture2D(const char* path, TextureType type);
            Texture2D& operator=(Texture2D&& texture) noexcept;
            Texture2D(Texture2D&& texture) noexcept;

            ~Texture2D();

            unsigned int getID();
            void bind();
            void bindUnit(const unsigned int unit) const;
            void unbindUnit() const;

        private:
            unsigned int _id = 0;
            unsigned char* _data;
            TextureType _type;
    };
}
