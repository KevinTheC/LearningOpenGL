#ifndef RESIZELISTENER_H
#define RESIZELISTENER_H
#include "includes.h"
class ResizeListener {
public:
	virtual void handleResize(GLFWwindow* window, int w, int h) = 0;
};
#endif
