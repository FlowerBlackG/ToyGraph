

#include "./NahidaScene.h"

#include <ToyGraph/Skybox.h>
#include <vector>
#include <string>
#include <ToyGraph/Scene/SceneManager.h>
#include <iostream>
#include <ToyGraph/Engine.h>

using namespace std;

static float lastX = 0;
static float lastY = 0;
static bool firstMouse = true;

void NahidaScene::cursorPosCallback(double xPos, double yPos) {
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

void NahidaScene::activeKeyInputProcessor(GLFWwindow* window, float deltaTime) {

    auto pCamera = SceneManager::getInstance().currentScene()->camera;
    auto& camera = *pCamera;
    auto& runtime = AppRuntime::getInstance();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 

    float cameraSpeed = 2.5f * deltaTime;
    
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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && runtime.lastFrameKeyStatus[GLFW_KEY_B] == GLFW_RELEASE) {
        SceneManager::getInstance().navigateBack();
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && runtime.lastFrameKeyStatus[GLFW_KEY_Z] == GLFW_RELEASE) {
        SceneManager::getInstance().navigateTo(NahidaScene::constructor);
    }
    
}

NahidaScene::~NahidaScene() {
    if (this->pSkybox) {
        delete this->pSkybox;
    }

    if (this->nahidaModel) {
        delete this->nahidaModel;
    }
}

void NahidaScene::tick(float deltaT) {
    auto nahida = this->actors[0];
    nahida->setYaw(nahida->getYaw() + deltaT * 20);

}


void NahidaScene::render() {
    auto& runtime = AppRuntime::getInstance();

    nahidaShader.use();
    
    auto view = camera->getViewMatrix();
    auto projection = glm::perspective(
        glm::radians(camera->getFov()), 
        1.0f * runtime.getWindowWidth() / runtime.getWindowHeight(), 
        0.1f, 
        100.0f
    );
    
    
    nahidaShader.setMatrix4fv("projection", projection)
        .setMatrix4fv("view", view);


    for (auto it : this->actors) {
        
        nahidaShader.setMatrix4fv("model", it.second->getModelMatrix());
        it.second->render(&nahidaShader);
    }

    pSkybox->render();
}


NahidaScene::NahidaScene() {
    vector<string> skyboxFaces({
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_right1.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_left2.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_top3.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_bottom4.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_front5.png",
        "../resources/SpaceboxCollection/Spacebox3/LightGreen_back6.png"
    });

    pSkybox = new Skybox(skyboxFaces);
    this->camera = new Camera;
    camera->setPosition(glm::vec3(-10, 2, 4));

    Actor* nahida = new Actor;
    nahida->setScale(glm::vec3(0.2));
    this->addActor(nahida);

    Actor* nahida2 = new Actor;
    nahida2->setScale(glm::vec3(0.2));
    nahida2->setPosition(glm::vec3(0, 0, 8));
    nahida2->setYaw(180);
    this->addActor(nahida2);

    nahidaModel = new Model("../resources/genshin/nahida/nahida.pmx");
    nahida->bindModel(nahidaModel);
    nahida2->bindModel(nahidaModel);

    if (nahidaShader.errcode != ShaderError::SHADER_OK) {
        cout << "nahida shader err: " << nahidaShader.errmsg << endl;
    }

}

