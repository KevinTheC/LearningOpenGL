#include "VAO.h"
VAO::VAO() {
	attribs = 0;
	total = 0;
	glGenVertexArrays(1, &ID);
	this->bind();
}
//i put comment ontop of the shader like a boss so it knows the layout :glasses:
//in different APIs it might be better to have a single vao per format but opengl 3.3
void VAO::linkAttribs(Shader* sh,VBO& vbo)
{
	attribs = 0;
	total = 0;
	vbo.bind();
	std::vector<int> vec = sh->getAttribs();
	int offset = 0;
	for (int i : vec){
		offset += i;
	}
	for (int i : vec)
	{
		glVertexAttribPointer(attribs, i, GL_FLOAT, GL_FALSE, offset * sizeof(float), (void*)(total*sizeof(float)));
		glEnableVertexAttribArray(attribs++);
		total += i;
	}
	vbo.unbind();
}
void VAO::bind()
{
	glBindVertexArray(ID);
}
void VAO::unbind()
{
	glBindVertexArray(0);
}
void VAO::erase()
{
	glDeleteVertexArrays(1,&ID);
}