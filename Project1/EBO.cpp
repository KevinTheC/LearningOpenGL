#include "EBO.h"
EBO::EBO(std::shared_ptr<std::vector<GLuint>> vec)
{
	indices = vec;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size()*sizeof(GLuint), indices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::erase()
{
	glDeleteBuffers(1, &ID);
}
void EBO::draw(Shader* sh)
{
	sh->activate();
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)(indices->data()));
}