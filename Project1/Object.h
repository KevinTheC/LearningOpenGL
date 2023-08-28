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
#include "Renderable.h"
#include "array"
#include <stb_image.h>
class Object : public Renderable
{
private:
	std::shared_ptr<std::vector<GLfloat>> vertices;
	std::shared_ptr<std::vector<GLuint>> indices;
	std::shared_ptr<VBO> vbo;
	std::shared_ptr<EBO> ebo;
	std::shared_ptr<VAO> vao;
	std::shared_ptr<Shader> sp;
	glm::mat4 model;
	int stride;
	void(*drawFunction)() = []() {};
public:
	Object(std::shared_ptr<Shader> sp, std::shared_ptr<std::vector<GLfloat>> verts, std::shared_ptr<std::vector<GLuint>> inds);

	const glm::mat4& getModel();
	const std::shared_ptr<Shader> getShader();

	const glm::vec3 center();
	void transform(glm::vec3 v3);

	void setContext(void(*func)());
	void draw();


	struct VertIterator {
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const glm::vec3;
		using pointer = const glm::vec3*;
		using reference = const glm::vec3&;
		int stride;
		VertIterator(pointer ptr, int stride) : m_ptr(ptr), stride(stride / 3) {}
		reference operator*() const { return *m_ptr; }
		pointer operator->() { return m_ptr; }

		VertIterator& operator++() { m_ptr += stride; return *this; }
		VertIterator& operator--() { m_ptr -= stride; return *this; }
		VertIterator operator++(int) { VertIterator tmp = *this; ++(*this); return tmp; }
		VertIterator operator--(int) { VertIterator tmp = *this; --(*this); return tmp; }

		friend bool operator== (const VertIterator& a, const VertIterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const VertIterator& a, const VertIterator& b) { return a.m_ptr != b.m_ptr; };
	public:
		glm::vec3* _Ptr() {
			return (glm::vec3*)m_ptr;
		}
	private:
		pointer m_ptr;
	};
	VertIterator vertbegin() {
		return VertIterator((glm::vec3*)vertices->data(), stride);
	}
	VertIterator vertend() {
		return VertIterator((glm::vec3*)(vertices->data() + vertices->size()), stride);
	}
	void editVertice(VertIterator loc, glm::vec3 newpoint);
	void eraseVertice(VertIterator loc);
	void addVertice(glm::vec3 newpoint);
};