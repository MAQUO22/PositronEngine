#include "PositronEngineCore/Material.hpp"
#include "PositronEngineCore/Log.hpp"

namespace PositronEngine
{

    Material::Material() :
        _name("default"),
        _base_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        _metallic(0.0f), _specular(0.3f), _roughness(16.0f),
        _diffuse_factor(1.0f), _ambient_factor(0.3f),
        _has_color_map(false), _has_normal_map(false),
        _has_roughness_map(false), _has_displacement_map(false)
    {
        _shader_program = std::make_shared<ShaderProgram>("default.vert", "default.frag", "default.geom");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        _shader_program->bind();

        _shader_program->setBool("has_color_map", _has_color_map);
        _shader_program->setBool("has_normal_map", _has_normal_map);
        _shader_program->setBool("has_roughness_map", _has_roughness_map);
        _shader_program->setBool("has_displacement_map", _has_displacement_map);

        _shader_program->setVec4("base_color", _base_color);
        _shader_program->setFloat("metallic", _metallic);
        _shader_program->setFloat("specular_factor", _specular);
        _shader_program->setFloat("roughness", _roughness);
        _shader_program->setFloat("diffuse_factor", _diffuse_factor);
        _shader_program->setFloat("ambient_factor", _ambient_factor);
    }

    Material::Material(std::string name, std::vector<Texture2D>& textures) :
        _name(name),
        _base_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        _metallic(0.0f), _specular(0.3f), _roughness(16.0f),
        _diffuse_factor(1.0f), _ambient_factor(0.3f),
        _has_color_map(false), _has_normal_map(false),
        _has_roughness_map(false), _has_displacement_map(false)
    {
        _shader_program = std::make_shared<ShaderProgram>("default.vert", "default.frag", "default.geom");
        if(!_shader_program->isCompile())
            LOG_CRITICAL("Material not compiled!");

        _textures = std::move(textures);

        LOG_INFORMATION("TEXTURES SIZE -> {0}", _textures.size());

        for(size_t i = 0; i < _textures.size(); i++)
        {
            if(_textures[i].getType() == TextureType::diffuse)
                _has_color_map = true;
            else if(_textures[i].getType() == TextureType::normal)
                _has_normal_map = true;
            else if(_textures[i].getType() == TextureType::roughness)
                _has_roughness_map = true;
            else if(_textures[i].getType() == TextureType::displasement)
                _has_displacement_map = true;
        }

        _shader_program->bind();

        _shader_program->setBool("has_color_map", _has_color_map);
        _shader_program->setBool("has_normal_map", _has_normal_map);
        _shader_program->setBool("has_roughness_map", _has_roughness_map);
        _shader_program->setBool("has_displacement_map", _has_displacement_map);

        _shader_program->setVec4("base_color", _base_color);
        _shader_program->setFloat("metallic", _metallic);
        _shader_program->setFloat("specular_factor", _specular);
        _shader_program->setFloat("roughness", _roughness);
        _shader_program->setFloat("diffuse_factor", _diffuse_factor);
        _shader_program->setFloat("ambient_factor", _ambient_factor);
    }

    void Material::setBaseColor(const glm::vec4 base_color)
    {
        getShaderProgram()->bind();

        if(!checkColorMap())
        {
            _base_color = base_color;

            getShaderProgram()->setBool("has_color_map", checkColorMap());
            getShaderProgram()->setVec4("base_color", _base_color);
        }

        else
            LOG_WARNING("THIS MATERIAL HAS COLOR MAP!");
    }

    glm::vec4 Material::getBaseColor()
    {
        if(!checkColorMap())
            return _base_color;
        else
        {
           LOG_WARNING("THIS MATERIAL HAS COLOR MAP!");
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    void Material::setMetallicFactor(const float metallic_factor)
    {
        getShaderProgram()->bind();

        if(metallic_factor >= 1.0f)
            _metallic = 1.0f;
        else if(metallic_factor <= 0.0f)
            _metallic = 0.0f;
        else
            _metallic = metallic_factor;

        getShaderProgram()->setFloat("metallic", _metallic);
    }

    float* Material::getMetallicFactor()
    {
        return &_metallic;
    }

    void Material::setSpecularFactor(const float specular_factor)
    {
        _specular = specular_factor;

        getShaderProgram()->bind();
        getShaderProgram()->setFloat("specular_factor", _specular);
    }

    float* Material::getSpecularFactor()
    {
        return &_specular;
    }

    void Material::setRoughnessFactor(const float roughness_factor)
    {
        getShaderProgram()->bind();

        if(!checkRoughnessMap())
        {
           _roughness = roughness_factor;

            getShaderProgram()->setBool("_has_roughness_map", checkRoughnessMap());
            getShaderProgram()->setFloat("roughness", _roughness);
        }
        else
            LOG_WARNING("THIS MATERIAL HAS ROUGHNESS MAP!");
    }

    float* Material::getRoughnessFactor()
    {
        if(!checkRoughnessMap())
            return &_roughness;
        else
        {
            LOG_WARNING("THIS MATERIAL HAS ROUGHNESS MAP!");
            return nullptr;
        }
    }

    void Material::setDiffuseFactor(const float diffuse_factor)
    {
        _diffuse_factor = diffuse_factor;

        getShaderProgram()->bind();
        getShaderProgram()->setFloat("diffuse_factor", _diffuse_factor);
    }

    float* Material::getDiffuseFactor()
    {
        return &_diffuse_factor;
    }

    void Material::setAmbientFactor(const float ambient_factor)
    {
        _ambient_factor = ambient_factor;

        getShaderProgram()->bind();
        getShaderProgram()->setFloat("ambient_factor", _ambient_factor);
    }

    float* Material::getAmbientFactor()
    {
        return &_ambient_factor;
    }

    bool Material::checkColorMap()
    {
        return _has_color_map;
    }

    bool Material::checkNormalMap()
    {
        return _has_normal_map;
    }

    bool Material::checkRoughnessMap()
    {
        return _has_roughness_map;
    }

    bool Material::checkDisplacementMap()
    {
        return _has_displacement_map;
    }

    //============================+ПОТОМ_ИСПРАВИТЬ!!======================================================

    void Material::setColorMap(Texture2D& color_map)
    {
        // _textures.push_back(Texture2D(color_map));
        // _has_color_map = true;
        //
        // getShaderProgram()->bind();
        // getShaderProgram()->setBool("has_color_map", _has_color_map);
    }

    void Material::setNormalMap(Texture2D& normal_map)
    {
        // _textures.push_back(Texture2D(normal_map));
        // _has_normal_map = true;
        //
        // getShaderProgram()->bind();
        // getShaderProgram()->setBool("has_normal_map", _has_normal_map);
    }

    void Material::setRoughnessMap(Texture2D& rougnes_map)
    {
        // _textures.push_back(Texture2D(rougnes_map));
        // _has_roughness_map = true;
        //
        // getShaderProgram()->bind();
        // getShaderProgram()->setBool("has_normal_map", _has_roughness_map);
    }

    void Material::setDisplacementMap(Texture2D& displacement_map)
    {
        // _textures.push_back(Texture2D(displacement_map));
        // _has_displacement_map = true;
    }

    //==============================================================================================================================

    void Material::bindMaterialMaps()
    {
        for(size_t i = 0; i < _textures.size(); i++)
        {
            if(_textures[i].getType() == TextureType::diffuse)
                _textures[i].bindUnit(0);

            else if(_textures[i].getType() == TextureType::roughness)
                _textures[i].bindUnit(1);

            else if(_textures[i].getType() == TextureType::normal)
                _textures[i].bindUnit(2);

            else if(_textures[i].getType() == TextureType::displasement)
                LOG_INFORMATION("DISP map");
        }
    }

    void Material::unbindMaterialMaps()
    {
        for(size_t i = 0; i < _textures.size(); i++)
        {
            _textures[i].unbindUnit();
        }
    }

    void Material::clearTextures()
    {
        _textures.clear();

        // _has_color_map = false;
        // _has_displacement_map = false;
        // _has_normal_map = false;
        // _has_roughness_map = false;
        //
        // getShaderProgram()->bind();
        // getShaderProgram()->setBool("has_color_map", _has_color_map);
        // getShaderProgram()->setBool("has_displacement_map", _has_displacement_map);
        // getShaderProgram()->setBool("has_normal_map", _has_normal_map);
        // getShaderProgram()->setBool("has_roughness_map", _has_roughness_map);

    }

    std::string Material::getName()
    {
        return _name;
    }

    std::shared_ptr<ShaderProgram> Material::getShaderProgram()
    {
        return _shader_program;
    }

    std::vector<Texture2D>& Material::getTextures()
    {
        return _textures;
    }
}
