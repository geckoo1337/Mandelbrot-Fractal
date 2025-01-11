#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
/////////////////////////////////////////////////
// declarations
/////////////////////////////////////////////////
class Shader
{
public:
	// the program ID
	GLuint ShaderProgramID;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	void deleteProgram()
	{
		glDeleteProgram(ShaderProgramID);
	}
	
	void drawCanvas()
	{
		glUseProgram(ShaderProgramID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, GLboolean value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (GLint)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, GLint value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, GLfloat value) const
	{
		glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, GLfloat x, GLfloat y) const
	{
		glUniform2f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
	{
		glUniform3f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};
/////////////////////////////////////////////////
// definitions
/////////////////////////////////////////////////
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{	// load shader from path
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	// Oups! something goes wrong. Path?
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// compile the shaders
	GLuint vertex;
	GLuint fragment;
	GLint success;
	GLchar infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	// done?
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	// done?
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// shader Program
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertex);
	glAttachShader(ShaderProgramID, fragment);
	glLinkProgram(ShaderProgramID);
	// eventual error
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
#endif
