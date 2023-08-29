#include "Axis.h"
class PointMovementManager
{
public:
	PointMovementManager(Object::VertIterator point, Object* ptr) : center(point) {
		this->ptr = ptr;
		box = CubeFactory::generate(*point - glm::vec3{.02f,.02f,.02f}, { .04f,.04f,.04f }, { 0.0f,0.0f,0.0f }, ptr->getShader());
		box->setContext([]() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
		x = new Axis(Axis::X, point, ptr->getShader());
		y = new Axis(Axis::Y, point, ptr->getShader());
		z = new Axis(Axis::Z, point, ptr->getShader());
	};
	Object::VertIterator center;
	Object* ptr;
	Object* box;
	Axis* x;
	Axis* y;
	Axis* z;
	~PointMovementManager() {
		delete(box);
		delete(x);
		delete(y);
		delete(z);
	}
};

