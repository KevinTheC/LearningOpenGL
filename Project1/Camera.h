#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "array"
#include "cmath"
#include "Object.h"
#include <stb_image.h>
class Camera
{
public:
	static std::shared_ptr<Camera> instance()
	{
		if (camera.get() == nullptr)
			camera = std::shared_ptr<Camera>(new Camera);
		return camera;
	}
	Camera(Camera const&) = delete;
	Camera& operator=(Camera const&) = delete;

	void linkShader(Shader* sh);
	void updateProjection(int w, int h);
	void setFocus(Object* focus);
	const glm::mat4& getProj();
	const glm::mat4& getView();


	static void GLFWmouseButtonCB(GLFWwindow *window, int button, int action, int mods);
	static void GLFWmouseMoveCB(GLFWwindow* window, double xnewpos, double ynewpos);
	static void GLFWresizeCB(GLFWwindow* window, int width, int height);
private:
	Camera();
	glm::mat4 proj;
	glm::mat4 view;
	static float distance(std::pair<float,float>&, std::pair<float,float>);
	static std::array<GLfloat, 3> getClosestTo(std::pair<float,float> pos,Object& o);
	static std::shared_ptr<Camera> camera;

	int width;
	int height;
	Object* focus;
	unsigned int uboMatrices;

	float thetaang;
	float omegaang;
	int flip;
	float prev;
	bool inc;
	double xpos;
	double ypos;
};

