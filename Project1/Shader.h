#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include "vector"
#include "array"
std::string get_file_contents(const char* filename);

class Shader
{
private:
	static GLuint current;
	std::vector<int> attribs;
public:
	const GLuint ID = glCreateProgram();
	Shader(const char* vertexFile, const char* fragmentFile);
	bool activate();
	void erase();
	void compileError(unsigned int shader, const char* type);
	std::vector<int> getAttribs();
};
#endif