
#include <ToyGraph/Engine.h>

#include <iostream>

#include <Windows.h>
#include "../OrbitDemo/Planet.h"

// stb image

#include <stb_image.h>

using namespace std;

const int SCR_WIDTH = 1200;
const int SCR_HEIGHT = 850;

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

GLuint loadCubemapMyImpl(const vector<string>& faces) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    int w, h, nrChannels;
    for (int faceIdx = 0; faceIdx < faces.size(); faceIdx++) {
        unsigned char* data = stbi_load(faces[faceIdx].c_str(), &w, &h, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIdx, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            exit(501);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
    GL_CLAMP_TO_EDGE);

    return textureId;

}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


int main(int argc, const char* argv[]) {


    string modelPath = "nahida/nahida.pmx";
    if (argc < 2) {
        //cout << "input model path " << endl << "> ";
        //cin >> modelPath;
    } else {
        modelPath = argv[1];
    }

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    AppRuntime& appRuntime = AppRuntime::getInstance("mesh loading", SCR_WIDTH, SCR_HEIGHT);

    camera.setPosition(glm::vec3(0, 0, 3));

    appRuntime.frameBufferSizeCallback = [&] (int w, int h) {
        appRuntime.setWindowSize(w, h);

    };

    //Model model("../resources/objects/backpack/backpack.obj");
    Model model(modelPath);
    if (model.errcode != ModelError::MODEL_OK) {
        cout << model.errmsg << endl;
    //    return -1;
    }

    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    if (shader.errcode != ShaderError::SHADER_OK) {
        cout << shader.errmsg << endl;
        return -2;
    }

    // skybox
    vector<string> faces = {
        "../resources/textures/skybox/right.jpg",
        "../resources/textures/skybox/left.jpg",
        "../resources/textures/skybox/top.jpg",
        "../resources/textures/skybox/bottom.jpg",
        "../resources/textures/skybox/front.jpg",
        "../resources/textures/skybox/back.jpg"
    };
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    if (skyboxShader.errcode != ShaderError::SHADER_OK) {
        cout << skyboxShader.errmsg << endl;
        exit(-1);
    }

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    GLuint cubemapTexture = loadCubemap(faces);
    cout << "cubemap texture: " << cubemapTexture << endl;

    GLuint skyboxVao, skyboxVbo;
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glBindVertexArray(skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    model.setScale(glm::vec3(0.2));
    //model.setPitch(45);
    //model.setYaw(45);

    appRuntime.cursorPosCallback = mouseCallback;
    appRuntime.activeKeyInputProcessor = processInput;

    appRuntime.tick = [&] (float deltaT) {


        auto view = camera.getViewMatrix();
        auto projection = glm::perspective(glm::radians(camera.getFov()), 1.0f * SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        // transform
        

        shader.setMatrix4fv("projection", projection)
            .setMatrix4fv("view", view);

        //auto model2 = glm::mat4(1);
        //shader.setMatrix4fv("model", model2);

            
        shader.setMatrix4fv("model", model.getModelMatrix());
        model.draw(shader);

        // skybox
        glDepthFunc(GL_LEQUAL);
        
        skyboxShader.use();
        auto skyView = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMatrix4fv("view", skyView);
        skyboxShader.setMatrix4fv("projection", projection);
        
        glBindVertexArray(skyboxVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
    };

    
    appRuntime.run();

    return 0;
}

