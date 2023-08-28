#ifndef MOUSEWHEELLISTENER_H
#define MOUSEWHEELLISTENER_H
#include "includes.h"
class MouseWheelListener {
public:
	virtual void handleMouseWheel(GLFWwindow* window, double xoffset, double yoffset) = 0;
};
#endif