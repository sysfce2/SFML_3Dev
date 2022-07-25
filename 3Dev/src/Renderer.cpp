#include <Renderer.hpp>

void Renderer::Init(sf::Window& w, std::string environmentMapFilename, Matrices& m, uint skyboxSideSize, uint irradianceSideSize, uint prefilteredSideSize)
{
    shaders[ShaderType::Main] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "vertex.vs", std::string(SHADERS_DIRECTORY) + "fragment.frag");
    shaders[ShaderType::Skybox] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "skybox.vs", std::string(SHADERS_DIRECTORY) + "skybox.frag");
    shaders[ShaderType::Depth] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "depth.vs", std::string(SHADERS_DIRECTORY) + "depth.frag");
    shaders[ShaderType::Post] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "post.vs", std::string(SHADERS_DIRECTORY) + "post.frag");
    shaders[ShaderType::Environment] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "environment.vs", std::string(SHADERS_DIRECTORY) + "environment.frag");
    shaders[ShaderType::Irradiance] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "irradiance.vs", std::string(SHADERS_DIRECTORY) + "irradiance.frag");
    shaders[ShaderType::Filtering] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "spcfiltering.vs", std::string(SHADERS_DIRECTORY) + "spcfiltering.frag");
    shaders[ShaderType::BRDF] = std::make_shared<Shader>(std::string(SHADERS_DIRECTORY) + "brdf.vs", std::string(SHADERS_DIRECTORY) + "brdf.frag");

    framebuffers[FramebufferType::Main] = std::make_shared<Framebuffer>(shaders[ShaderType::Post].get(), w.getSize().x, w.getSize().y);
    framebuffers[FramebufferType::Transparency] = std::make_shared<Framebuffer>(shaders[ShaderType::Post].get(), w.getSize().x, w.getSize().y);

    Framebuffer capture(nullptr, skyboxSideSize, skyboxSideSize), captureIrr(nullptr, irradianceSideSize, irradianceSideSize),
                captureSpc(nullptr, prefilteredSideSize, prefilteredSideSize), captureBRDF(shaders[ShaderType::BRDF].get(), 512, 512);

    Material environmentMaterial(
    {
        { LoadHDRTexture(environmentMapFilename), Material::Type::Environment }
    });

    Shape captureCube({ 500, 500, 500 }, &environmentMaterial, shaders[ShaderType::Environment].get(), &m, nullptr);
    GLuint cubemap = capture.CaptureCubemap(captureCube, m);
    textures[TextureType::Skybox] = cubemap;

    environmentMaterial.GetParameters().clear();
    environmentMaterial.AddParameter(cubemap, Material::Type::Cubemap);
    
    captureCube.SetShader(shaders[ShaderType::Irradiance].get());
    GLuint irr = captureIrr.CaptureCubemap(captureCube, m, true);
    textures[TextureType::Irradiance] = irr;

    captureCube.SetShader(shaders[ShaderType::Filtering].get());
    GLuint filtered = captureSpc.CaptureCubemapMipmaps(captureCube, m, 8, 1024);
    textures[TextureType::Prefiltered] = filtered;

    captureBRDF.Capture(0);
    GLuint BRDF = captureBRDF.GetTexture();
    textures[TextureType::LUT] = BRDF;
}

GLuint Renderer::GetTexture(TextureType type)
{
    return textures[type];
}

Shader* Renderer::GetShader(ShaderType type)
{
    return shaders[type].get();
}

Framebuffer* Renderer::GetFramebuffer(FramebufferType type)
{
    return framebuffers[type].get();
}

void Renderer::SetupMaterial(Material& mat)
{
    if(!mat.Contains(Material::Type::Irradiance)) mat.AddParameter(GetTexture(TextureType::Irradiance), Material::Type::Irradiance);
	if(!mat.Contains(Material::Type::PrefilteredMap)) mat.AddParameter(GetTexture(TextureType::Prefiltered), Material::Type::PrefilteredMap);
	if(!mat.Contains(Material::Type::LUT)) mat.AddParameter(GetTexture(TextureType::LUT), Material::Type::LUT);
}