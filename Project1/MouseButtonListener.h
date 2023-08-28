#ifndef MOUSEBUTTONLISTENER_H
#define MOUSEBUTTONLISTENER_H
#include "includes.h"
class MouseButtonListener {
public:
	virtual void handleMouseButton(GLFWwindow* window, int button, int action, int mods) = 0;
};
#endif