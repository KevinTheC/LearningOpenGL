#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "array"
#include "UniqueID.h"
#include <stb_image.h>
class Object
{
private:
	static UniqueID IDgen;
	//this will need to be heavily optimized
	static std::vector<Object*> drawables;
	//this isnt even used
	long int ID;


	std::shared_ptr<std::vector<GLfloat>> vertices;
	std::shared_ptr<std::vector<GLuint>> indices;
	std::shared_ptr<VBO> vbo;
	std::shared_ptr<EBO> ebo;
	std::shared_ptr<VAO> vao;
	std::shared_ptr<Shader> sp;
	glm::mat4 model;
	int gap;
	void(*drawFunction)() = [](){};
public:
	Object(std::shared_ptr<Shader> sp, std::shared_ptr<std::vector<GLfloat>> verts, std::shared_ptr<std::vector<GLuint>> inds);
	
	
	const glm::mat4& getModel();
	const std::shared_ptr<Shader> getShader();

	const long int getID();
	static bool existID(long int num);

	const glm::vec3 center();
	void transform(glm::vec3 v3);

	void setContext(void(*func)());
	const void draw();
	const static void drawAll();
	~Object();
	

	struct ConstIterator {
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const std::array<GLfloat, 3>;
		using pointer = const std::array<GLfloat, 3>*;
		using reference = const std::array<GLfloat, 3>&;
		int gap;
		ConstIterator(pointer ptr, int gap) : m_ptr(ptr), gap(gap / 3) {}
		reference operator*() const { return *m_ptr; }
		pointer operator->() { return m_ptr; }

		ConstIterator& operator++() { m_ptr += gap; return *this; }
		ConstIterator& operator--() { m_ptr -= gap; return *this; }
		ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }
		ConstIterator operator--(int) { ConstIterator tmp = *this; --(*this); return tmp; }

		friend bool operator== (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		pointer m_ptr;
	};
	ConstIterator cbegin() {
		return ConstIterator((std::array<GLfloat, 3>*)vertices->data(), gap);
	}
	ConstIterator cend() {
		return ConstIterator((std::array<GLfloat, 3>*)(vertices->data() + vertices->size()), gap);
	}
};