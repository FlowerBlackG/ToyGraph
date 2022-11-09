// π‚’’ ‘—È°£

#include <ToyGraph/Engine.h>

// stl
#include <iostream>

#include <Windows.h>

// stb image
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

const unsigned SCR_WIDTH = 1800;
const unsigned SCR_HEIGHT = 1200;

Camera camera;

float lastX = 0;
float lastY = 0;
bool isFirstMouse = true;

// timing.
float deltaTime = 0;
float lastFrameTime = 0;

// lighting
glm::vec3 lightPos(1.2, 1, 2);

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint loadCubemap(const vector<string>& faces) {
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

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isFirstMouse) {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = false;
        return;
    }

    float xOff = xpos - lastX;
    float yOff = lastY - ypos; // reversed: y ranges bottom to top.
    lastX = xpos;
    lastY = ypos;

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


void processInput(GLFWwindow* window) {
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

void prepareCamera() {
    camera.setPosition(glm::vec3(0, 0, 3));
    camera.setYaw(-90);
}

int main() {

    // glfw initialization and configuration.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "lighting demo", nullptr, nullptr);

    if (window == nullptr) {
        cout << "failed to create window." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "failed to init glad" << endl;
        return -1;
    }

    // camera
    prepareCamera();

    // opengl global settings
    glEnable(GL_DEPTH_TEST);

    // shaders
    Shader lightingShader("shaders/colors.vs", "shaders/moreLight.fs");

    if (lightingShader.errcode != ShaderError::SHADER_OK) {
        cout << lightingShader.errmsg << endl;
        exit(-200);
    }

    Shader lightCubeShader("shaders/lightCube.vs", "shaders/lightCube.fs");

    if (lightCubeShader.errcode != ShaderError::SHADER_OK) {
        cout << lightCubeShader.errmsg << endl;
        exit(-201);
    }

    // vertices' data
    float vertices[] = {
        // positions // normals // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };


    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // normal cube
    GLuint VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // light source
    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    GLuint specularMap = TextureUtils::loadTextureFromFile("../resources/textures/container2_specular.png");

    cout << "specularMap id: " << specularMap << endl;
    if (specularMap < 0) {
        return -1;
    }

    GLuint diffuseMap = TextureUtils::loadTextureFromFile("../resources/textures/container2.png");
    
    cout << "diffuseMap id: " << diffuseMap << endl;
    if (diffuseMap < 0) {
        return -1;
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
    skyboxShader.setInt("skybox", 0);

    GLuint cubemapTexture = loadCubemap(faces);    

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
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // render loop.
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currFrameTime = glfwGetTime();
        deltaTime = currFrameTime - lastFrameTime;
        lastFrameTime = currFrameTime;

        // input
        processInput(window);

        // render begin //
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        auto view = camera.getViewMatrix();
        auto projection = glm::perspective(glm::radians(camera.getFov()), 1.0f * SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        


        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2);
        lightColor.y = sin(glfwGetTime() * 0.7);
        lightColor.z = sin(glfwGetTime() * 1.3);

        lightColor = glm::vec3(2, 2, 2);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        lightingShader.use();

        lightingShader.setInt("material.diffuse", 0)
            .setInt("material.specular", 1)
            .setVector3f("light.position", camera.getPosition())
            .setVector3f("light.direction", camera.getDirectionVectorFront())
            .setFloat("light.cutOff", glm::cos(glm::radians(12.5f)))
            .setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)))
            .setFloat("light.constant", 1)
            .setFloat("light.linear", 0.09)
            .setFloat("light.quadratic", 0.032)
            ;

        lightingShader.setVector3f("material.specular", 0.5, 0.5, 0.5)
            .setFloat("material.shininess", 64.0f)
            .setVector3f("light.ambient", ambientColor)
            .setVector3f("light.diffuse", diffuseColor)
            .setVector3f("light.specular", 1, 1, 1)
            .setVector3f("lightPos", lightPos);

        lightingShader.setVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f)
            .setVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f)
            .setVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f)
            .setVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f)
            ;

        
        lightingShader.setVector3f("pointLights[0].position", pointLightPositions[0])
            .setVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f)
            .setVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f)
            .setVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f)
            .setFloat("pointLights[0].constant", 1.0f)
            .setFloat("pointLights[0].linear", 0.09f)
            .setFloat("pointLights[0].quadratic", 0.032f)
            ;
        lightingShader.setVector3f("pointLights[1].position", pointLightPositions[1])
            .setVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f)
            .setVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f)
            .setVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f)
            .setFloat("pointLights[1].constant", 1.0f)
            .setFloat("pointLights[1].linear", 0.09f)
            .setFloat("pointLights[1].quadratic", 0.032f)
            ;
        lightingShader.setVector3f("pointLights[2].position", pointLightPositions[2])
            .setVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f)
            .setVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f)
            .setVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f)
            .setFloat("pointLights[2].constant", 1.0f)
            .setFloat("pointLights[2].linear", 0.09f)
            .setFloat("pointLights[2].quadratic", 0.032f)
            ;
        lightingShader.setVector3f("pointLights[3].position", pointLightPositions[3])
            .setVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f)
            .setVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f)
            .setVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f)
            .setFloat("pointLights[3].constant", 1.0f)
            .setFloat("pointLights[3].linear", 0.09f)
            .setFloat("pointLights[3].quadratic", 0.032f)
            ;

        // transform
        

        
        lightingShader.setMatrix4fv("projection", projection)
            .setMatrix4fv("view", view);

        auto model = glm::mat4(1);
        lightingShader.setMatrix4fv("model", model)
            .setVector3f("viewPos", camera.getPosition());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);

        // wooden boxes

        for (int i = 0; i < 10; i++) {

            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 0.3, 0.5));

            lightingShader.setMatrix4fv("model", model);

            // render the cube
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // draw the lamp object

        for (int i = 0; i < 4; i++) {
        
            lightCubeShader.use();
            lightCubeShader.setMatrix4fv("projection", projection)
                .setMatrix4fv("view", view);

            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2));
            lightCubeShader.setMatrix4fv("model", model);

            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        auto skyboxView = glm::mat4(glm::mat3(view));
        skyboxShader.setMatrix4fv("projection", projection)
            .setMatrix4fv("view", skyboxView);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // render end //

        // swap buffers and poll io events;
        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(2);
    }

    // clean up.
    glfwTerminate();

    return 0;
}
