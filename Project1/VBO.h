#pragma once
#ifndef VBO_CLASS_H
#define VBO_CLASS_H
#include <GL/glew.h>
#include "vector"
#include "memory"
class VBO
{
public:
	GLuint ID;
	VBO(std::shared_ptr<std::vector<GLfloat>>);
	void bind();
	void unbind();
	void erase();
	void update();
private:
	std::shared_ptr<std::vector<GLfloat>> vertices;
};
#endif

