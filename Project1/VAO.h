#pragma once
#ifndef VAO_CLASS_H
#define VAO_CLASS_H
#include <GL/glew.h>
#include "VBO.h"
#include "Shader.h"
class VAO
{
public:
	static GLuint boundID;
	GLuint ID;
	VAO();
	void linkAttribs(Shader* sh, VBO& vbo);
	void bind();
	void unbind();
	void erase();
private:
	int attribs;
	int total;
};
#endif
