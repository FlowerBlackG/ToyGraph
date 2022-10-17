
#include <ToyGraph/Engine.h>

#include <iostream>

#include <Windows.h>
using namespace std;

const int SCR_WIDTH = 1800;
const int SCR_HEIGHT = 1200;

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


int main() {
    AppRuntime& appRuntime = AppRuntime::getInstance("mesh loading", 1800, 1200);

    camera.setPosition(glm::vec3(0, 0, 3));

    appRuntime.frameBufferSizeCallback = [&] (int w, int h) {
        appRuntime.setWindowSize(w, h);

    };

    //Model model("../resources/objects/backpack/backpack.obj");
    Model model("Nahida/Nahida.pmx");
    assert(model.errcode == ModelError::MODEL_OK);

    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    assert(shader.errcode == ShaderError::SHADER_OK);

    appRuntime.cursorPosCallback = mouseCallback;
    appRuntime.activeKeyInputProcessor = processInput;


    appRuntime.tick = [&] (float deltaT) {
        
        shader.use();
        // transform
        auto projection = glm::perspective(glm::radians(camera.getFov()), 1.0f * SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        auto view = camera.getViewMatrix();
        shader.setMatrix4fv("projection", projection)
            .setMatrix4fv("view", view);

        auto model2 = glm::mat4(1);
        shader.setMatrix4fv("model", model2);
        model.draw(shader);
        
    };

    
    appRuntime.run();

    return 0;
}
