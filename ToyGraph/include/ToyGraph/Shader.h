/*
 * Shader 程序封装类。
 */

#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum class ShaderError {
    SHADER_OK,
    V_SHADER_OPEN_FAILED,
    F_SHADER_OPEN_FAILED,
    V_SHADER_COMPILE_FAILED,
    F_SHADER_COMPILE_FAILED,
    LINKING_FAILED,
};

/**
 * Shader 程序封装类。 
 */
class Shader {
public:
    Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

    

    ~Shader() {};

    GLuint getId() { return id; }
    GLuint setId(GLuint id) { 
        this->id = id; 
    }

    /**
     * 使用这个 shader 程序。 
     */
    void use() {
        glUseProgram(id);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), int(value));
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

public:

    /**
     * 错误码。执行危险操作（如构造）后应检查此值。
     */
    ShaderError errcode = ShaderError::SHADER_OK;

    /**
     * 错误信息。
     */
    std::string errmsg = "";

private:

    /**
     * 初始化。使用构造函数调用此函数，可以更方便地在中途退出。
     */
    void init(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

private:

    /**
     * shader 程序id。
     */
    GLuint id = -1;
};
