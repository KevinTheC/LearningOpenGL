#pragma once
#include "includes.h"
class KeyListener {
public:
	virtual void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};