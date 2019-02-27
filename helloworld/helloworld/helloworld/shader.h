#pragma once

#ifndef SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
private:
	int createFragmentShader(const char* fragmentShaderSource);
	int createVertexShader(const char* vertexShaderSource);
	int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
};


#endif // !SHADER_H
