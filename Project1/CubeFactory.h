#ifndef CUBEFACTORY_H
#define CUBEFACTORY_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Object.h"
#define black 0.0f, 0.0f, 0.0f,
class CubeFactory
{
public:
	static Object generate(glm::vec3 center, float width, std::array<GLfloat, 3> color,std::shared_ptr<Shader> sh)
	{
		float farx =center[0] + width * .5f;
		float fary = center[1] + width * .5f;
		float farz = center[2] + width * .5f;
		Object* o = new Object(sh, std::shared_ptr<std::vector<GLfloat>>(new std::vector<GLfloat>(
			{farx,fary,farz,0.0f, 0.0f, 0.0f,
			farx,fary,farz-width,0.0f, 0.0f, 0.0f,
			farx,fary-width,farz,0.0f, 0.0f, 0.0f,
			farx,fary-width,farz-width,0.0f, 0.0f, 0.0f,
			farx-width,fary,farz,0.0f, 0.0f, 0.0f,
			farx-width,fary,farz-width,0.0f, 0.0f, 0.0f,
			farx-width,fary - width,farz,0.0f, 0.0f, 0.0f,
			farx-width,fary - width,farz - width,0.0f, 0.0f, 0.0f,
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
		o->setContext([]() {glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); });
		return *o;
	}
};
#endif

