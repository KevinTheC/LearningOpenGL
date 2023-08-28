#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include "includes.h"
#include "vector"
#include "listeners.h"
class InputController
{
private:
	static std::vector<DragListener*> draggers;
	static std::vector<ResizeListener*> resizers;
	static std::vector<MouseButtonListener*> mbListeners;
	static std::vector<MouseWheelListener*> mwListeners;
	template <typename T>
	static void remove(T* t, std::vector<T*> vec) {
		for (int i = 0; i < vec.size(); i++)
			if (t == vec.at(i))
			{
				vec.erase(vec.begin() + i);
				return;
			}
	}
public:
	template <typename T>
	static void addObserver(T* t) {
		if(dynamic_cast<DragListener*>(t))
			draggers.push_back(t);
		if (dynamic_cast<ResizeListener*>(t))
			resizers.push_back(t);
		if (dynamic_cast<MouseButtonListener*>(t))
			mbListeners.push_back(t);
	}
	template <typename T>
	static void removeObserver(T* t) {
		if (dynamic_cast<DragListener*>(t))
			remove(t, draggers);
		if (dynamic_cast<ResizeListener*>(t))
			remove(t, resizers);
		if (dynamic_cast<MouseButtonListener*>(t))
			remove(t, mbListeners);
	}


	static void GLFWmouseMoveCB(GLFWwindow* window, double xnewpos, double ynewpos)
	{
		for (DragListener* dl : draggers)
			dl->handleDrag(window, xnewpos, ynewpos);
	}
	static void GLFWresizeCB(GLFWwindow* window, int w, int h)
	{
		for (ResizeListener* rl : resizers)
			rl->handleResize(window, w, h);
	}
	static void GLFWmouseButtonCB(GLFWwindow* window, int button, int action, int mods)
	{
		for (MouseButtonListener* mbl : mbListeners)
			mbl->handleMouseButton(window, button, action, mods);
	}
	static void GLFWkeyCB(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			break;
		}
	}
	static void GLFWmouseWheelCB(GLFWwindow* window, double xoffset, double yoffset)
	{
		for (MouseWheelListener* mw : mwListeners)
			mw->handleMouseWheel(window, xoffset, yoffset);
	}
};
#endif