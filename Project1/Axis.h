#include "Object.h"
#include "DragListener.h"
#include "MouseButtonListener.h"
#include "includes.h"
#define red 1.0f, 0.0f, 0.0f,
#define green 0.0f, 1.0f, 0.0f,
#define blue 0.0f, 0.0f, 1.0f,
class Axis : public DragListener, public MouseButtonListener, public Renderable{
public:
	typedef enum xyz {
		X,
		Y,
		Z,
	};
	Axis(xyz dire, const Object::VertIterator var) : itr(var), direction(dire)
	{
		focused = false;
	};
	virtual void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
	virtual void handleDrag(GLFWwindow* window, double x, double y);
	virtual void draw();
private:
	const Object::VertIterator itr;
	bool focused;
	xyz direction;
	std::shared_ptr<Object> me;
};