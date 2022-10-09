#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Windows.h>
#include <cmath>

#include "ToyGraph/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ToyGraph
#include <ToyGraph/Camera.h>

using namespace std;

#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1200


void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

// camera
Camera camera;

// camera - rotation
float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;
bool isFirstMouse = true;

// deltaT
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void prepareCamera() {
    camera.setPosition(glm::vec3(0, 0, 3))
        .setYaw(-90.0f);
    
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

void scrollCallback(GLFWwindow* window, double xoff, double yoff) {
    float fov = camera.getFov();
    fov -= yoff;
    if (fov < 1) {
        fov = 1;
    } else if (fov > 45) {
        fov = 45;
    }
    camera.setFov(fov);

    cout << camera.getFov() << endl;
}

int main(int argc, const char* argv[]) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象。
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        cout << "failed to create glfw window." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 检查 glad。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "failed to init glad." << endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 控制鼠标。
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetScrollCallback(window, scrollCallback);

    prepareCamera();

    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

    if (ourShader.errcode != ShaderError::SHADER_OK) {
        cout << "[error] " << ourShader.errmsg << endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    // 准备 vertex data
#if 0
    float vertices[] = {
        /* x, y, z, [-1, 1] */ /* R, G, B */
    /*	 -0.5,  -0.5, 0, 1,   0,   0,
          0,  0.5, 0, 0,   1,   0,
          0.5,  -0.5, 0, 0,   0,   1,*/

         0.5,  0.5, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0,
         0.5, -0.5, 0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
        -0.5, -0.5, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0,
        -0.5,  0.5, 0.0,   1.0, 1.0, 0.0,   0.0, 1.0

    };
#endif

    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    unsigned indices[] = {
        0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 3, 7, 0, 1, 4, 
        1, 4, 5, 1, 5, 6, 1, 2, 6, 3, 6, 7, 3, 2, 6, 7, 6, 4, 4, 5, 6
    };

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 准备 vertex buffer。
    GLuint VBO; // vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 拷贝到缓存。
    /*
      第三个参数：显卡处理数据的方式。
        GL_STREAM_DRAW: 少次使用
        GL_STATIC_DRAW: 多次使用
        GL_DYNAMIC_DRAW: 多变、多次使用
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);

    stbi_set_flip_vertically_on_load(true);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "failed to load texture." << endl;
        exit(-201);
    }
    stbi_image_free(data);

    // texture of happy face
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "failed to load texture 2." << endl;
        exit(-202);
    }


    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.getId(), "texture1"), 0);
    ourShader.setInt("texture2", 1);

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    
    vec = trans * vec;

    glm::mat4 model = glm::mat4(1);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1, 0, 0));
    
    lastFrame = glfwGetTime();

    // light
    glm::vec3 lightColor(1, 1, 1);
    glm::vec3 toyColor(1, 0.5, 0.31);
    auto resultColor = lightColor * toyColor;

    
    // light vao
    


    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glm::vec3 lightPos(1.2, 1, 2.0);
    auto lightModel = glm::mat4(1);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2));
    

    // 渲染循环。
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);	
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);



        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0, 0, 1));
        ourShader.setMatrix4fv("transform", trans);


        //float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 view = camera.getViewMatrix();

        auto projection = glm::perspective(glm::radians(camera.getFov()), 1.0f * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

        ourShader.setMatrix4fv("model", model)
        .setMatrix4fv("view", view)
        .setMatrix4fv("projection", projection);

        for (int i = 0; i < 10; i++) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 0.3, 0.5));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5, 1, 0));

            ourShader.setMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();

        // cout << "deltaT: " << deltaTime << endl;
        // cout << glfwGetTime() << endl;

        if (deltaTime < 0.008) {
            Sleep(8 - deltaTime * 1000);
        }
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
