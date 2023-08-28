#include "Object.h"
Object::Object(std::shared_ptr<Shader> sp, std::shared_ptr<std::vector<GLfloat>> verts, std::shared_ptr<std::vector<GLuint>> inds)
{
	this->sp = sp;
	vertices = verts;
	indices = inds;
	model = glm::mat4(1.0f);

	sp->activate();
	std::vector<int> vec = sp->getAttribs();
	stride = 0;
	for (int& i : vec)
		stride += i;
	vbo = std::shared_ptr<VBO>(new VBO(vertices));
	vao = std::shared_ptr<VAO>(new VAO());
	ebo = std::shared_ptr<EBO>(new EBO(indices));
	vao->linkAttribs(sp.get(), *vbo.get());
	vao->unbind();
	vbo->unbind();
	ebo->unbind();
}
void Object::setContext(void(*func)())
{
	drawFunction = func;
}
void Object::transform(glm::vec3 v3)
{
	model = glm::translate(model, v3);
}
void Object::editVertice(VertIterator loc, glm::vec3 newpoint) {
	int point = (GLfloat*)loc._Ptr() - vertices.get()->data();
	vertices->at(point) = newpoint[0];
	vertices->at(point + 1) = newpoint[1];
	vertices->at(point + 2) = newpoint[2];
	vbo->update();
}
void Object::eraseVertice(VertIterator loc) {
	if (vertices->size() == stride)
		return;
	int point = (GLfloat*)loc._Ptr() - vertices.get()->data();
	for (int i = 0; i < stride; i++) {
		vertices->erase(vertices->begin() + point);
	}
	vbo->update();
}
void Object::addVertice(glm::vec3 newpoint) {
	//under construction
}
glm::vec3 const Object::center()
{
	auto centerPoint = glm::vec3();
	for (int i = 0; i < 3; i++)
	{
		float f = 0.0f;
		for (int j = 0; j < vertices->size(); j += stride)
			f += vertices->at(j + i);
		f /= stride;
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


void Object::draw()
{
	drawFunction();
	int modelLoc = glGetUniformLocation(sp->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	vao->bind();
	ebo->draw(sp.get());
	vao->bind();
}