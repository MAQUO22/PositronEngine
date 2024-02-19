#pragma once
#include "PositronEngineCore/stb_image.h"
#include <string>

namespace PositronEngine
{
    class CubeMapTexture
    {
        public:
            CubeMapTexture(CubeMapTexture&);
            CubeMapTexture& operator=(const CubeMapTexture&) = delete;

            CubeMapTexture(const std::string* paths);

            CubeMapTexture& operator=(CubeMapTexture&& texture) noexcept;
            CubeMapTexture(CubeMapTexture&& texture) noexcept;

            ~CubeMapTexture();

            unsigned int getID();
            void bind();
            void bindUnit(const unsigned int unit) const;
            void unbindUnit() const;

        private:
            unsigned int _id = 0;
            unsigned char* _data;
    };
}

