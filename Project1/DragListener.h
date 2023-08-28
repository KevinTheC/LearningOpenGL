#ifndef DRAGLISTENER_H
#define DRAGLISTENER_H
#include "includes.h"
class DragListener {
public:
	virtual void handleDrag(GLFWwindow* window,double x, double y) = 0;
};
#endif