
#include "./PlanetScene.h"
#include "./Planet.h"
#include <ToyGraphCommon/Random.h>
#include <ToyGraph/Engine.h>
#include <iostream>

using namespace std;

static float lastX = 0;
static float lastY = 0;
static bool firstMouse = true;

void PlanetScene::cursorPosCallback(double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    auto pCamera = SceneManager::getInstance().currentScene()->camera;
    auto& camera = *pCamera;
    

    float xOff = xPos - lastX;
    float yOff = lastY - yPos; // reversed: y ranges bottom to top.
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    xOff *= sensitivity;
    yOff *= sensitivity;

    float pitch = camera.getPitch();
    float yaw = camera.getYaw();

    pitch += yOff;
    yaw += xOff;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    camera.setPitch(pitch);
    camera.setYaw(yaw);
}

void PlanetScene::activeKeyInputProcessor(GLFWwindow* window, float deltaTime) {

    auto pCamera = SceneManager::getInstance().currentScene()->camera;
    auto& camera = *pCamera;
    auto& runtime = AppRuntime::getInstance();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 

    float cameraSpeed = 24 * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        camera.move(cameraSpeed, camera.getDirectionVectorFront());
  
    } 

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        
        camera.move(-cameraSpeed, camera.getDirectionVectorFront());
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(-cameraSpeed, camera.getDirectionVectorRight());
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(cameraSpeed, camera.getDirectionVectorRight());
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.move(cameraSpeed, camera.getDirectionVectorUp());
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.move(-cameraSpeed, camera.getDirectionVectorUp());
    }
    
}

PlanetScene::~PlanetScene() {

    cout << "PlanetScene::~PlanetScene obj: " << this << endl;
    for (auto it : this->models) {
        delete it;
    }

    if (this->camera) {
        delete this->camera;
    }
}

void PlanetScene::render() {
    auto& runtime = AppRuntime::getInstance();
    auto projection = glm::perspective(
        glm::radians(camera->getFov()), 
        1.0f * runtime.getWindowWidth() / runtime.getWindowHeight(), 
        0.1f, 100.0f
    );

    auto view = camera->getViewMatrix();
    shader.use();
    shader.setMatrix4fv("projection", projection)
        .setMatrix4fv("view", view);

    // 光线设置。
    glm::vec3 lightColor(1, 1, 1);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    shader.setVector3f("light.position", camera->getPosition())
        .setVector3f("light.direction", camera->getDirectionVectorFront())
        .setFloat("light.cutOff", glm::cos(glm::radians(12.5f)))
        .setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)))
        .setFloat("light.constant", 1)
        .setFloat("light.linear", 0.09)
        .setFloat("light.quadratic", 0.032)
        .setVector3f("material.specular", 0.5, 0.5, 0.5)
        .setFloat("material.shininess", 64.0f)
        .setVector3f("light.ambient", ambientColor)
        .setVector3f("light.diffuse", diffuseColor)
        .setVector3f("light.specular", 1, 1, 1)
        .setVector3f("lightPos", glm::vec3(1.0f));
    ;

    Scene::render();
}

static void __tgLoadGenshinModels(vector<Model*>& container) {
    container.push_back(new Model("../resources/genshin/collei/collei.pmx"));
    container.push_back(new Model("../resources/genshin/nahida/nahida.pmx"));
    container.push_back(new Model("../resources/genshin/paimon/paimon.pmx"));
    container.push_back(new Model("../resources/genshin/sayu/sayu.pmx"));
}

PlanetScene::PlanetScene() {

    if (this->shader.errcode != ShaderError::SHADER_OK) {
        cout << "shader error: " << this->shader.errmsg << endl;
    }

    // 准备天空盒。
    vector<string> skyboxFaces({
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_right1.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_left2.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_top3.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_bottom4.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_front5.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_back6.png"
    });

    pSkybox = new Skybox(skyboxFaces);

    // 相机。
    this->camera = new Camera;
    camera->setPosition(glm::vec3(-51.4599, 22.9925, -0.314531));
    camera->setPitch(-30);

    // 模型。
    __tgLoadGenshinModels(this->models);

    vector<Actor*> moons;

    // 准备星球们。
    for (int planetIdx = 0; planetIdx < 10; planetIdx++) {

        Planet* planet = new Planet;
        
        moons.push_back(planet);
        
        planet->setScale(glm::vec3(0.32));
        
        planet->orbitRadius = planetIdx * 5;

        glm::vec3 colorFilter = glm::vec3(0, 0, 0);
        colorFilter.r = Random::randInt(40, 255) / 255.0f;
        colorFilter.g = Random::randInt(40, 255) / 255.0f;
        colorFilter.b = Random::randInt(40, 255) / 255.0f;
 
        planet->colorFilter = colorFilter;

        planet->revolutionRotation = Random::randInt(0, 360);
        planet->revolutionSpeed = Random::randInt(12, 36);
        planet->selfRotationSpeed = Random::randInt(32, 120);

        // 9 号设为 3 号的卫星。
        if (planetIdx == 9) {
            planet->setScale(glm::vec3(0.12));
            planet->orbitRadius = 2.6;
            planet->revolutionSpeed = 360;
            moons.pop_back();
            moons[2]->addChild(planet);
        }

        planet->bindModel(this->models[
            Random::randInt(0, this->models.size() - 1)
        ]);

    }

    for (auto it : moons) {
        this->addActor(it);
        it->setShader(this->shader.getId());
    }

    this->addActor(this->pSkybox);
    
    
}
