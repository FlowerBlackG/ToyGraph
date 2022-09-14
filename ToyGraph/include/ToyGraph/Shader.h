/*
 * Shader �����װ�ࡣ
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
 * Shader �����װ�ࡣ 
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
     * ʹ����� shader ���� 
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
     * �����롣ִ��Σ�ղ������繹�죩��Ӧ����ֵ��
     */
    ShaderError errcode = ShaderError::SHADER_OK;

    /**
     * ������Ϣ��
     */
    std::string errmsg = "";

private:

    /**
     * ��ʼ����ʹ�ù��캯�����ô˺��������Ը����������;�˳���
     */
    void init(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

private:

    /**
     * shader ����id��
     */
    GLuint id = -1;
};
