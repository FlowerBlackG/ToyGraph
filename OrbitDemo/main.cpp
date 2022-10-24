/*
    同济计算机系图形学课程作业2：行星模拟。
    by 2051565
*/

#include <ToyGraph/Engine.h>
#include <ToyGraphCommon/Random.h>
#include <iostream>
#include <vector>

#include "Planet.h"

using namespace std;

const int SCR_WIDTH = 1400;
const int SCR_HEIGHT = 900;

Camera camera;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool firstMouse = true;
void mouseCallback(double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

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

void processInput(GLFWwindow* window, float deltaTime) {
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
}


int main(const int argc, const char* argv[]) {

    AppRuntime& runtime = AppRuntime::getInstance("Orbit Demo", SCR_WIDTH, SCR_HEIGHT);

    runtime.cursorPosCallback = mouseCallback;
    runtime.frameBufferSizeCallback = [&] (int w, int h) {
        runtime.setWindowSize(w, h);
    };
    runtime.activeKeyInputProcessor = processInput;

    camera.setPosition(glm::vec3(-51.4599, 22.9925, -0.314531));
    camera.setPitch(-30);

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    vector<Actor*> actors;

    Model moonModel("planets/moon.pmx");

    // 准备星球们。
    for (int planetIdx = 0; planetIdx < 10; planetIdx++) {


        Planet* planet = new Planet;
        
        actors.push_back(planet);
        
        planet->setScale(glm::vec3(0.18));
        
        planet->orbitRadius = planetIdx * 5;

        glm::vec3 colorFilter = glm::vec3(0, 0, 0);
        colorFilter.r = Random::randInt(40, 255) / 255.0f;
        colorFilter.g = Random::randInt(40, 255) / 255.0f;
        colorFilter.b = Random::randInt(40, 255) / 255.0f;
 
        planet->colorFilter = colorFilter;

        planet->revolutionRotation = Random::randInt(0, 360);
        planet->revolutionSpeed = Random::randInt(32, 80);
        planet->selfRotationSpeed = Random::randInt(32, 120);

        // 9 号设为 3 号的卫星。
        if (planetIdx == 9) {
            planet->setScale(glm::vec3(0.12));
            planet->orbitRadius = 2.6;
            planet->revolutionSpeed = 360;
        }

        planet->tick = [=] (float deltaT) {

            if (planetIdx == 9) {
                planet->rotationCenter = actors[3]->getPosition();
            }

            planet->updateTransform(deltaT);
        };

        planet->render = [=, &shader, &moonModel] () {

            shader.setVector3f("colorFilter", planet->colorFilter)
                .setMatrix4fv("model", planet->getModelMatrix());

            moonModel.draw(shader);
        };

    }

    shader.use();
    runtime.tick = [&] (float deltaSeconds) {
        
        for (auto actor : actors) {
            actor->tick(deltaSeconds);
        }

        auto projection = glm::perspective(
            glm::radians(camera.getFov()), 
            1.0f * runtime.getWindowWidth() / runtime.getWindowHeight(), 
            0.1f, 100.0f
        );

        auto view = camera.getViewMatrix();
        shader.setMatrix4fv("projection", projection)
            .setMatrix4fv("view", view);

        for (auto actor : actors) {
            actor->render();
        }

    };

    runtime.run();

    // cleanup.
    for (auto& actor : actors) {
        delete actor;
    }

    return 0;
}

