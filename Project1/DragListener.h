#pragma once
#include "includes.h"
class DragListener {
public:
	virtual void handleDrag(GLFWwindow* window,double x, double y) = 0;
};