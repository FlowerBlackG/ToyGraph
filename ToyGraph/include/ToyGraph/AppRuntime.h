/*
    ToyGraph Runtime Header
    created on 2022.10.17
*/

#pragma once

#include <ToyGraph/Engine.h>

enum class AppRuntimeError {
    RUNTIME_OK,
    FAILED_LOAD_GLAD,
    FAILED_CREATE_WINDOW
};

/**
 * ToyGraph 运行时控制器。
 * 不严格限制单例，但只应生成 1 个实例。否则会产生 UB。
 */
class AppRuntime {
public:
    AppRuntime();
    AppRuntime(
        const std::string& windowTitle = "",
        int windowWidth = 1800,
        int windowHeight = 1200
    );

    ~AppRuntime();

    /**
     * 初始化 glfw。
     */
    void prepareGlfw();

    /**
     * 创建一个窗口，同时准备 glad。
     */
    void createWindow(
        const std::string& title = "",
        int width = 1800, 
        int height = 1200
    );

public: // setters & getters.

    AppRuntime& setWindowWidth(int width);
    AppRuntime& setWindowHeight(int height);
    int getWindowHeight() { return windowHeight; }
    int getWindowWidth() { return windowWidth; }

public:
    AppRuntimeError errcode = AppRuntimeError::RUNTIME_OK;
    std::string errmsg;


protected:
    GLFWwindow* window = nullptr;
    int windowWidth;
    int windowHeight;

private:
    AppRuntime(const AppRuntime&) = default;

};

