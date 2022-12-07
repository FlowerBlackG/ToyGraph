
#pragma once

#include <ToyGraphCommon/EngineCompileOptions.h>
#include <ToyGraph/Scene/Scene.h>
#include <ToyGraph/Scene/SceneManager.h>
#include <ToyGraph/Skybox.h>
#include <ToyGraph/Shader.h>
#include <ToyGraph/Model/Model.h>
#include <string>
#include <vector>

class PlanetScene : public Scene {
public:
    ~PlanetScene();

    static PlanetScene* constructor() {
        return new PlanetScene;
    }

    virtual void render() override;

    PlanetScene();

    void cursorPosCallback(double xPos, double yPos) override;

    void activeKeyInputProcessor(GLFWwindow* window, float deltaT) override;

    Skybox* pSkybox = nullptr;

    Shader shader{"shaders/shader.vert", "shaders/shader.frag"};
    
    std::vector<Model*> models;
};
