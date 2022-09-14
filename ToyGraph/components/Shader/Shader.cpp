
#include "ToyGraph/Shader.h"

using namespace std;

Shader::Shader(const string& vertexShaderFilePath, const string& fragmentShaderFilePath) {
	this->init(vertexShaderFilePath, fragmentShaderFilePath);
}

void Shader::init(const string& vertexShaderFilePath, const string& fragmentShaderFilePath) {

	// 打开文件。
	ifstream vShaderFile(vertexShaderFilePath, ios::binary);
	ifstream fShaderFile(fragmentShaderFilePath, ios::binary);

	if (!vShaderFile.is_open()) {
		errmsg = "failed to open vertex shader file.";
		errcode = ShaderError::V_SHADER_OPEN_FAILED;
		return;
	}

	if (!fShaderFile.is_open()) {
		errmsg = "failed to open fragment shader file.";
		errcode = ShaderError::F_SHADER_OPEN_FAILED;
		return;
	}

	stringstream vShaderStream;
	stringstream fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	string stdVShaderCode = vShaderStream.str();
	string stdFShaderCode = fShaderStream.str();
	const char* vShaderCode = stdVShaderCode.c_str();
	const char* fShaderCode = stdFShaderCode.c_str();

	GLuint vertexId;
	GLuint fragmentId;
	int success;
	char infoLog[512];

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vShaderCode, nullptr);
	glCompileShader(vertexId);
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexId, sizeof(infoLog) / sizeof(char), nullptr, infoLog);
		errcode = ShaderError::V_SHADER_COMPILE_FAILED;
		errmsg = "failed to compile vertex shader. ";
		errmsg += infoLog;
		return;
	}

	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentId);
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentId, sizeof(infoLog), nullptr, infoLog);
		errcode = ShaderError::F_SHADER_COMPILE_FAILED;
		errmsg = "failed to compile fragment shader. ";
		errmsg += infoLog;
		return;
	}

	this->id = glCreateProgram();
	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
		errcode = ShaderError::LINKING_FAILED;
		errmsg = "failed to link program. ";
		errmsg += infoLog;
		return;
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	vShaderFile.close();
	fShaderFile.close();

}
