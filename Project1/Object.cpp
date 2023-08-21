#include "Object.h"
//unused
UniqueID Object::IDgen;

std::vector<Object*> Object::drawables;
Object::Object(std::shared_ptr<Shader> sp, std::shared_ptr<std::vector<GLfloat>> verts, std::shared_ptr<std::vector<GLuint>> inds)
{
	this->sp = sp;
	this->ID = IDgen.genID();
	vertices = verts;
	indices = inds;
	model = glm::mat4(1.0f);

	sp->activate();
	std::vector<int> vec = sp->getAttribs();
	gap = 0;
	for (int& i : vec)
		gap += i;

	vbo = std::shared_ptr<VBO>(new VBO(vertices));
	vao = std::shared_ptr<VAO>(new VAO());
	ebo = std::shared_ptr<EBO>(new EBO(indices));
	vao->linkAttribs(sp.get(),*vbo.get());
	vao->unbind();
	vbo->unbind();
	ebo->unbind();
	drawables.push_back(this);
}
void Object::setContext(void(*func)())
{
	drawFunction = func;
}
void Object::transform(glm::vec3 v3)
{
	model = glm::translate(model,v3);
}
glm::vec3 const Object::center()
{
	auto centerPoint = glm::vec3();
	for (int i = 0; i < 3; i++)
	{
		float f = 0.0f;
		for (int j = 0; j < vertices->size(); j += gap)
			f += vertices->at(j + i);
		f /= gap;
		centerPoint[i] = f;
	}
	return centerPoint;
}

const glm::mat4& Object::getModel()
{
	return model;
}
const std::shared_ptr<Shader> Object::getShader()
{
	return sp;
}
const long int Object::getID()
{
	return ID;
}
bool Object::existID(long int num)
{
	return IDgen.existsID(num);
}


void const Object::draw()
{
	drawFunction();
	int modelLoc = glGetUniformLocation(sp->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	vao->bind();
	ebo->draw(sp.get());
	vao->bind();
}
void const Object::drawAll()
{
	for (int i = 0; i < drawables.size(); i++)
		drawables.at(i)->draw();
}
Object::~Object()
{
	for (auto itr = drawables.begin(); itr != drawables.end(); ++itr)
		if (*itr == this)
		{
			drawables.erase(itr);
			break;
		}
	IDgen.freeID(ID);
}