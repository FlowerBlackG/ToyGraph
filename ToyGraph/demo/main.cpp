#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Windows.h>

#include "ToyGraph/Shader/Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, const char* argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口对象。
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	/*
	// 准备 vertex shader。
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	Shader vertexShaderObj = Shader("shaders/shader.vert");
	const char* vertexShaderSrc = vertexShaderObj.getBuffer();
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		cout << infoLog << endl;
		exit(-600);
	}


	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	auto fragmentShaderObj = Shader("shaders/shader.frag");
	const char* fragShaderSrc = fragmentShaderObj.getBuffer();
	glShaderSource(fragmentShader, 1, &fragShaderSrc, nullptr);
	glCompileShader(fragmentShader);

	// 绑定shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	// 清除 shader 对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/

	Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

	if (ourShader.errcode != ShaderError::SHADER_OK) {
		cout << "[error] " << ourShader.errmsg << endl;
		exit(-1);
	}

	// 准备 vertex data
	float vertices[] = {
		/* x, y, z, [-1, 1] */ /* R, G, B */
		 0.5,  0.5, 0.0, 1,   0,   0,
		 0.5, -0.5, 0.0, 0,   1,   0,
		-0.5, -0.5, 0.0, 0,   0,   1,
		-0.5,  0.5, 0.0, 1, 0.3, 0.7
	};

	unsigned indices[] = {
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 渲染循环。
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0f + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		ourShader.use();
		
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		
		

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(6);
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
