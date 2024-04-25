#pragma once

#include <vector>

#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Texture2D.hpp"

#include <memory>

namespace PositronEngine
{
    class Material
    {
        public:
            Material();

            Material(std::vector<Texture2D>& textures);
            ~Material() = default;

            void setBaseColor(const glm::vec4 base_color);
            glm::vec4 getBaseColor();

            void setMetallicFactor(const float metallic_factor);
            float* getMetallicFactor();

            void setSpecularFactor(const float specular_factor);
            float* getSpecularFactor();

            void setRoughnessFactor(const float roughness_factor);
            float* getRoughnessFactor();

            void setDiffuseFactor(const float diffuse_factor);
            float* getDiffuseFactor();

            void setAmbientFactor(const float ambient_factor);
            float* getAmbientFactor();

            bool checkColorMap();
            bool checkNormalMap();
            bool checkRoughnessMap();
            bool checkDisplacementMap();

            void setColorMap(Texture2D& color_map);
            void setNormalMap(Texture2D& normal_map);
            void setRoughnessMap(Texture2D& rougnes_map);
            void setDisplacementMap(Texture2D& displacement_map);

            void bindMaterialMaps();
            void unbindMaterialMaps();

            std::shared_ptr<ShaderProgram> getShaderProgram();
            std::vector<Texture2D>& getTextures();
            void clearTextures();

        private:
            glm::vec4 _base_color;
            float _metallic;
            float _specular;
            float _roughness;

            float _diffuse_factor;
            float _ambient_factor;

            bool _has_color_map;
            bool _has_normal_map;
            bool _has_roughness_map;
            bool _has_displacement_map;

            std::shared_ptr<ShaderProgram> _shader_program;
            std::vector<Texture2D> _textures;
    };
}
