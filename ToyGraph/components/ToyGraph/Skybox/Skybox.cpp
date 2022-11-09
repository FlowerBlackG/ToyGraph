/*

    Ìì¿ÕºÐ

    ´´½¨£º2022.11.7

*/

#include <ToyGraph/Skybox.h>
#include <ToyGraph/Scene/Scene.h>
#include <ToyGraph/Scene/SceneManager.h>
#include <ToyGraph/Engine.h>

#include <iostream>
#include <stb_image.h>

using namespace std;

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
            cout << "failed to load skybox texture: "
                << faces[faceIdx] << endl;
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

static const float skyboxVertices[] = {
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

Skybox::Skybox(const vector<string>& faces) {
    cout << "check point sk2" << endl;
    if (skyboxShader.errcode != ShaderError::SHADER_OK) {
        cout << "skybox shader error: " << skyboxShader.errmsg << endl;
        throw "skybox shader error!";
    }

    cout << "check point sk1" << endl;

    cubemapTexture = loadCubemap(faces);
    
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glBindVertexArray(skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
};



void Skybox::render(class Shader* pShader) {
    Scene* currScene = SceneManager::getInstance().currentScene();


    if (currScene == nullptr || currScene->camera == nullptr) {
        return;
    }

    auto& camera = *(currScene->camera);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    glDepthFunc(GL_LEQUAL);
    
    skyboxShader.use();
    auto skyView = glm::mat4(glm::mat3(camera.getViewMatrix()));
    skyboxShader.setMatrix4fv("view", skyView);

    int screenW = AppRuntime::getInstance().getWindowWidth();
    int screenH = AppRuntime::getInstance().getWindowHeight();

    auto projection = glm::perspective(glm::radians(camera.getFov()), 1.0f * screenW / screenH, 0.1f, 100.0f);
    skyboxShader.setMatrix4fv("projection", projection);
    
    glBindVertexArray(skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

}

