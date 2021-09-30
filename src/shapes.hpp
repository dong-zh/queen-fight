#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <chicken3421/chicken3421.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace shapes
{
	struct vertex_t {
		glm::vec4 position;
		glm::vec2 textureCoordinates;
	};

	struct rect_t {
		GLuint vao;
		GLuint vbo;
		std::vector<vertex_t> vertices;
		GLsizeiptr nVerts = 6;
	};

	rect_t makeRectangle(float xMin, float yMin, float xMax, float yMax);
} // namespace shapes

#endif	// SHAPES_HPP
