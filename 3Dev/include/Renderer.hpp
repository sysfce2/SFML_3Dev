#pragma once
#include "Utils.hpp"
#include "Shape.hpp"
#include "Framebuffer.hpp"

class Renderer
{
public:
    enum class TextureType
    {
        Skybox,
        Irradiance,
        Prefiltered,
        LUT
    };

    enum class ShaderType
    {
        Main,
        Skybox,
        Depth,
        Post,
        Environment,
        Irradiance,
        Filtering,
        BRDF
    };

    enum class FramebufferType
    {
        Main,
        Transparency
    };

    void Init(sf::Window& w, std::string environmentMapFilename, Matrices& m, uint skyboxSideSize = 256, uint irradianceSideSize = 32, uint prefilteredSideSize = 256);

    GLuint GetTexture(TextureType type);
    Shader* GetShader(ShaderType type);
    Framebuffer* GetFramebuffer(FramebufferType type);

    void SetupMaterial(Material& mat);

private:
    std::unordered_map<TextureType, GLuint> textures;
    std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaders;
    std::unordered_map<FramebufferType, std::shared_ptr<Framebuffer>> framebuffers;
};