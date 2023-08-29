#include "CubeFactory.h"
#include "DragListener.h"
#include "MouseButtonListener.h"
#include "includes.h"
#define red {1.0f, 0.0f, 0.0f}
#define green {0.0f, 1.0f, 0.0f}
#define blue {0.0f, 0.0f, 1.0f}
class Axis : public DragListener, public MouseButtonListener {
public:
	enum xyz {
		X,
		Y,
		Z,
	};
	Axis(xyz dire, const Object::VertIterator var,std::shared_ptr<Shader> sh) : itr(var), direction(dire)
	{
		focused = false;
		vector[dire] = .4f;
		*var - vector;
		switch (dire) {
		case X:
			me = CubeFactory::generate(*var - vector, {0.8f,0.005f,0.005f}, red, sh);
			break;
		case Y:
			me = CubeFactory::generate(*var - vector, { 0.005f,0.8f,0.005f }, green, sh);
			break;
		case Z:
			me = CubeFactory::generate(*var - vector, { 0.005f,0.005f,0.8f }, blue, sh);
			break;
		}
		me->setContext([]() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);});
	};
	~Axis() {
		delete(me);
	}
	virtual void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
	virtual void handleDrag(GLFWwindow* window, double x, double y);
	virtual void draw();
private:
	const Object::VertIterator itr;
	bool focused;
	xyz direction;
	glm::vec3 vector;
	Object* me;
};