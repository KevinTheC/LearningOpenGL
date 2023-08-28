#include "InputController.h"
std::vector<DragListener*> InputController::draggers;
std::vector<ResizeListener*> InputController::resizers;
std::vector<MouseButtonListener*> InputController::mbListeners;
std::vector<MouseWheelListener*> InputController::mwListeners;