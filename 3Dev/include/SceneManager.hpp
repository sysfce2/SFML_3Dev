#pragma once
#include "Model.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Framebuffer.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"

class SceneManager
{
public:
    void Draw(Framebuffer* fbo = nullptr, Framebuffer* transparency = nullptr, bool updatePhysics = true, bool shadowPass = false);
    void UpdatePhysics(bool updateThisFrame = true, bool updateModelTransforms = false);
    void UpdateAnimations();

    void AddModel(std::shared_ptr<Model> model, const std::string& name = "model", bool checkUniqueness = true);
    void AddMaterial(std::shared_ptr<Material> material, const std::string& name = "material");
    void AddAnimation(std::shared_ptr<Animation> animation, const std::string& name = "animation");
    void AddLight(std::shared_ptr<Light> light, const std::string& name = "light");

    void StoreBones(std::shared_ptr<Model> model, Bone* bone = nullptr);
    void RemoveBones(std::shared_ptr<Model> model, Bone* bone = nullptr);

    template<class... Args>
    std::shared_ptr<Model> CreateModel(const std::string& name, Args&&... args);

    template<class... Args>
    std::shared_ptr<Light> CreateLight(const std::string& name, Args&&... args);

    std::shared_ptr<Material> CreateMaterial(const std::string& name);

    void RemoveModel(std::shared_ptr<Model> model);
    void RemoveMaterial(std::shared_ptr<Material> material);
    void RemoveAnimation(std::shared_ptr<Animation> animation);
    void RemoveLight(std::shared_ptr<Light> light);

    void RemoveAllObjects();

    void Save(const std::string& filename, bool relativePaths = false);
    void Load(const std::string& filename, bool loadEverything = false);

    void SaveState();
    void LoadState();

    void LoadEnvironment(const std::string& filename);

    void SetMainShader(Shader* shader, bool temp = false);
    void SetCamera(Camera* camera);
    void SetSkybox(std::shared_ptr<Model> skybox);
    void SetPhysicsManager(std::shared_ptr<PhysicsManager> manager);
    void SetSoundManager(std::shared_ptr<SoundManager> manager);

    void SetModelName(const std::string& name, const std::string& newName);
    void SetMaterialName(const std::string& name, const std::string& newName);
    void SetAnimationName(const std::string& name, const std::string& newName);
    void SetLightName(const std::string& name, const std::string& newName);

    void SetUpdatePhysics(bool update);

    std::string GetLastAdded();

    std::shared_ptr<Model> GetModel(const std::string& name);
    std::shared_ptr<Light> GetLight(const std::string& name);
    std::shared_ptr<Material> GetMaterial(const std::string& name);
    std::shared_ptr<Animation> GetAnimation(const std::string& name);
    std::shared_ptr<PhysicsManager> GetPhysicsManager();
    std::shared_ptr<SoundManager> GetSoundManager();

    Node* GetNode(const std::string& name);

    std::string GetModelName(std::shared_ptr<Model> model);
    std::string GetModelName(Model* model);
    std::string GetNodeName(Node* node);
    std::string GetMaterialName(std::shared_ptr<Material> mat);
    std::string GetMaterialName(Material* mat);

    std::vector<std::shared_ptr<Model>> GetModelGroup(const std::string& name);

    // For angelscript
    Model* GetModelPtr(const std::string& name);
    Light* GetLightPtr(const std::string& name);
    Material* GetMaterialPtr(const std::string& name);
    Animation* GetAnimationPtr(const std::string& name);
    PhysicsManager* GetPhysicsManagerPtr();
    SoundManager* GetSoundManagerPtr();

    Model* CreateModelPtr(const std::string& filename = "", bool isTemporary = true, const std::string& name = "model");
    Light* CreateLightPtr(bool isTemporary = true, const std::string& name = "light");
    Material* CreateMaterialPtr(const std::string& name = "material");

    void RemoveModelPtr(Model* model);
    void RemoveLightPtr(Light* light);
    void RemoveMaterialPtr(Material* material);
    void RemoveAnimationPtr(Animation* animation);

    Model* CloneModel(Model* model, bool isTemporary = true, const std::string& name = "model");
    Light* CloneLight(Light* light, bool isTemporary = true, const std::string& name = "light");

    std::vector<Model*> GetModelPtrGroup(const std::string& name);

    Camera* GetCamera();
    Bone* GetBone(const std::string& name);

    std::vector<Light*> GetShadowCastingLights();

	// @return array of names, 0 - models, 1 - materials, 2 - lights, 3 - nodes, 4 - bones, 5 - animations
    std::array<std::vector<std::string>, 6> GetNames();

private:
    struct State
    {
        rp3d::Vector3 pos, size;
        rp3d::Quaternion orient;
    };

    void RemoveFromTheGroup(const std::string& group, std::shared_ptr<Model> model);
    void MoveToTheGroup(const std::string& from, const std::string& to, std::shared_ptr<Model> model);

    void UpdateLights(Shader* shader);

    std::pair<std::string, std::string> ParseName(const std::string& in);

    sf::Clock clock;

    Camera* camera;

    bool updatePhysics = true;

    float physicsTime = 0.0;

    std::string lastAdded = "";

    std::shared_ptr<Model> skybox;
    std::shared_ptr<SoundManager> sManager;
    std::shared_ptr<PhysicsManager> pManager;

    std::vector<std::string> temporaryModelCopies, temporaryLightCopies;

    std::map<std::string, std::vector<std::shared_ptr<Model>>> modelGroups = { { "decals", {} } };

    std::map<std::string, Node*> nodes;
    std::map<std::string, Bone*> bones;
    std::map<std::string, std::shared_ptr<Model>> models;
    std::map<std::string, std::shared_ptr<Material>> materials; // for editor and scene saving
    std::map<std::string, std::shared_ptr<Light>> lights;

    std::map<std::string, std::shared_ptr<Animation>> animations;

    std::vector<Node*> lightsVector; // for drawing

    std::map<std::string, State> savedState;

    Json::Value root;
};
