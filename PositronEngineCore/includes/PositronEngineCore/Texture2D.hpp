#pragma once
#include "PositronEngineCore/stb_image.h"

namespace PositronEngine
{
    enum class TextureType
    {
        diffuse,
        roughness,
        normal,
        displasement,
        buffer,
        shadow
    };

    class Texture2D
    {
        public:
            Texture2D(Texture2D&);
            Texture2D& operator=(const Texture2D&) = delete;

            Texture2D(const unsigned int width, const unsigned int height, TextureType texture_type);
            Texture2D(const char* path, TextureType type);
            Texture2D& operator=(Texture2D&& texture) noexcept;
            Texture2D(Texture2D&& texture) noexcept;

            ~Texture2D();

            unsigned int getID();
            TextureType getType();

            unsigned int getWidth() const;
            unsigned int getHeight() const;

            void bind();
            void bindUnit(const unsigned int unit) const;
            void unbindUnit() const;



        private:
            unsigned int _id = 0;
            unsigned char* _data;

            unsigned int _width;
            unsigned int _height;

            TextureType _type;
    };
}
