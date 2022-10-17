/*
    ToyGraph Runtime Impl
    created on 2022.10.17
*/

#include <ToyGraph/AppRuntime.h>

using namespace std;

/* ------------ init & deconstructor ------------ */

AppRuntime::AppRuntime() {
    this->prepareGlfw();
}

AppRuntime::AppRuntime(
    const string& windowTitle,
    int windowWidth,
    int windowHeight
) {
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

void AppRuntime::createWindow(
    const string& title,
    int width, 
    int height
) {
    // ½¡¿µ×´Ì¬¼ì²é¡£
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
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        this->errcode = AppRuntimeError::FAILED_LOAD_GLAD;
        this->errmsg = "failed to load glad.";
        return;
    }
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