/*
    ToyGraph Runtime Impl
    created on 2022.10.17
*/
#include "ToyGraphCommon/EngineCompileOptions.h"
#include <ToyGraph/AppRuntime.h>
#include <ToyGraph/Scene/SceneManager.h>
#include <ToyGraph/Scene/Scene.h>

#include <Windows.h>
#include <iostream>

using namespace std;

static void __tgPrintToyGraphLogo() {
    const char* logo = 
        "  _______           _____                 _     \n"
        " |__   __|         / ____|               | |    \n"
        "    | | ___  _   _| |  __ _ __ __ _ _ __ | |__  \n"
        "    | |/ _ \\| | | | | |_ | '__/ _` | '_ \\| '_ \\ \n"
        "    | | (_) | |_| | |__| | | | (_| | |_) | | | |\n"
        "    |_|\\___/ \\__, |\\_____|_|  \\__,_| .__/|_| |_|\n"
        "              __/ |                | |          \n"
        "             |___/                 |_|          ";

    cout << logo << "\n\n";
}

AppRuntime& AppRuntime::getInstance() {
    if (pInstance == nullptr) {
        pInstance = new AppRuntime;
    }

    return *pInstance;
}

AppRuntime& AppRuntime::getInstance(
    const string& title,
    int width, 
    int height
) {
    if (pInstance == nullptr) {
        pInstance = new AppRuntime(title, width, height);
    }


    return *pInstance;
}

void AppRuntime::createWindow(
    const string& title,
    int width, 
    int height
) {
    // 健康状态检查。
    if (errcode != AppRuntimeError::RUNTIME_OK) {
        return;
    }

    this->setWindowHeight(height);
    this->setWindowWidth(width);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        this->errcode = AppRuntimeError::FAILED_CREATE_WINDOW;
        this->errmsg = "failed to create window.";
        return;
    }

    glfwMakeContextCurrent(window);

    // 绑定 event callbacks。

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallbackBridge);
    glfwSetCursorPosCallback(window, cursorPosCallbackBridge);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        this->errcode = AppRuntimeError::FAILED_LOAD_GLAD;
        this->errmsg = "failed to load glad.";
        return;
    }

    glEnable(GL_DEPTH_TEST);
    
    // blending.
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* ------------ setters & getters. ------------ */

AppRuntime& AppRuntime::setWindowWidth(int width) {
    this->windowWidth = width;
    if (window) {
        glViewport(0, 0, this->windowWidth, this->windowHeight);
    }
    return *this;
}

AppRuntime& AppRuntime::setWindowHeight(int height) { 
    this->windowHeight = height;
    if (window) {
        glViewport(0, 0, this->windowWidth, this->windowHeight);
    }
    return *this;
}

AppRuntime& AppRuntime::setWindowSize(int width, int height) {
    this->windowHeight = height;
    this->windowWidth = width;
    if (window) {
        glViewport(0, 0, this->windowWidth, this->windowHeight);
    }
    return *this;
}

AppRuntime& AppRuntime::setTargetFrameRate(int target) {
    int realTarget = target < 1 ? 1 : target;

    this->targetFrameRate = realTarget;
    this->targetFrameTimeMs = 1000 / realTarget;

    return *this;
}


AppRuntime& AppRuntime::setTargetFrameTimeMs(int target) {
    int realTarget = min(max(0, target), 500);

    this->targetFrameTimeMs = realTarget;
    this->targetFrameRate = 1000 / realTarget;

    return *this;
}

/* ------------ tick control. ------------ */

void AppRuntime::run() {

    auto& sceneMgr = SceneManager::getInstance();
    if (sceneMgr.currentScene() == nullptr) {
        return;
    }
    
    // 健康状态检查。
    if (errcode != AppRuntimeError::RUNTIME_OK || window == nullptr) {
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        
        // 时间控制。
        
        float currentFrameTime = glfwGetTime();
        float deltaT = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // 处理输入。
        sceneMgr.currentScene()->activeKeyInputProcessor(window, deltaT);

        for (int idx = 0; idx < GLFW_KEY_LAST; idx++) {
            this->lastFrameKeyStatus[idx] = glfwGetKey(window, idx);
        }

        // 清空上一帧内容。
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 心跳。
        sceneMgr.currentScene()->tick(deltaT);
        sceneMgr.currentScene()->render();
    
        // 帧率控制。
        int deltaTMs = deltaT * 1000;
        int sleepTimeMs = targetFrameTimeMs - deltaTMs;

        if (sleepTimeMs > 0) {
            Sleep(sleepTimeMs);
        }

        // 交换帧和拉取操作。
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
}

/* ------------ event callback bridges. ------------ */

void AppRuntime::frameBufferSizeCallbackBridge(
    GLFWwindow* window, int width, int height
) {

    AppRuntime::getInstance().frameBufferSizeCallback(width, height);
    
}

void AppRuntime::cursorPosCallbackBridge(
    GLFWwindow* window, double xPos, double yPos
) {

    Scene* scene = SceneManager::getInstance().currentScene();
    if (scene != nullptr) {
        scene->cursorPosCallback(xPos, yPos);
    }

}


/* ------------ init & deconstructor ------------ */


AppRuntime* AppRuntime::pInstance = nullptr;


AppRuntime::AppRuntime() {
    __tgPrintToyGraphLogo();
    this->prepareGlfw();
}

AppRuntime::AppRuntime(
    const string& windowTitle,
    int windowWidth,
    int windowHeight
) {

    __tgPrintToyGraphLogo();

    this->prepareGlfw();
    this->createWindow(windowTitle, windowWidth, windowHeight);
}

AppRuntime::~AppRuntime() {
    glfwTerminate();
}

void AppRuntime::prepareGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

