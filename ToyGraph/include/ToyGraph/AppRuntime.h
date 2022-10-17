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
 * ToyGraph ����ʱ��������
 * ���ϸ����Ƶ�������ֻӦ���� 1 ��ʵ������������ UB��
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
     * ��ʼ�� glfw��
     */
    void prepareGlfw();

    /**
     * ����һ�����ڣ�ͬʱ׼�� glad��
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

