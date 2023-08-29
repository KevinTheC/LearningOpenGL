#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Object.h"
#define colors color[0],color[1],color[2],
class CubeFactory
{
public:
	static Object* generate(glm::vec3 center, glm::vec3 dimensions, glm::vec3 color, std::shared_ptr<Shader> sh)
	{
		float farx = center[0] + dimensions[0];
		float fary = center[1] + dimensions[1];
		float farz = center[2] + dimensions[2];
		Object* o = new Object(sh, std::shared_ptr<std::vector<GLfloat>>(new std::vector<GLfloat>(
			{ farx,fary,farz, colors
			farx,fary,farz - dimensions[2], colors
			farx,fary - dimensions[1],farz, colors
			farx,fary - dimensions[1],farz - dimensions[2], colors
			farx - dimensions[0],fary,farz, colors
			farx - dimensions[0],fary,farz - dimensions[2], colors
			farx - dimensions[0],fary - dimensions[1],farz, colors
			farx - dimensions[0],fary - dimensions[1],farz - dimensions[2], colors
			})), std::shared_ptr<std::vector<GLuint>>(new std::vector<GLuint>(
				{ 0U, 1U, 2U,
				1U, 2U, 3U,
				0U, 1U, 4U,
				1U, 4U, 5U,
				5U, 3U, 1U,
				5U, 3U, 7U,
				2U, 3U, 7U,
				6U, 2U, 7U,
				4U, 5U, 6U,
				6U, 5U, 7U,
				0U, 2U, 6U,
				0U, 4U, 6U
				})));
		o->setContext([]() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
		return o;
	}
};