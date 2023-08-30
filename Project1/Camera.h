#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "OpenGLIncludes.h"
#include "array"
#include "cmath"
#include "Object.h"
#include "PointMovementManager.h"
#include "CubeFactory.h"
#include "listeners.h"
class Camera : public DragListener, public MouseButtonListener, public ResizeListener, public MouseWheelListener, public KeyListener
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
	float subtractFromOne(float);


	glm::vec3 panDirection(double xdiff, double ydiff);
	void handleMouseWheel(GLFWwindow* window, double xoffset, double yoffset);
	void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
	void handleDrag(GLFWwindow* window, double xnewpos, double ynewpos);
	void handleResize(GLFWwindow* window, int width, int height);
	void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	void refresh();
private:
	Camera();
	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 center;
	glm::vec3 viewpoint;
	glm::vec2 toXY(glm::mat4 model, glm::vec4 point);
	static float distance(std::pair<float,float>&, std::pair<float,float>);
	Object::VertIterator getClosestTo(std::pair<float,float> pos,Object& o);
	static std::shared_ptr<Camera> camera;
	PointMovementManager* pmm;

	float yperc;
	int width;
	int height;
	Object* focus;
	unsigned int uboMatrices;
	float total;
	float omegaang;
	double xpos;
	double ypos;
};
#endif
