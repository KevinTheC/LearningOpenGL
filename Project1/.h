#pragma once
#include "vector"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
class TriangleFactory
{
public:
	static std::vector<GLfloat> generateTriangle(Shader* sh);
};

