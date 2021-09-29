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

	struct circle_t {
		GLuint vao = 0;
		GLuint vbo = 0;
		GLsizeiptr nVerts = 0;

		std::vector<glm::vec4> positions;
		std::vector<glm::vec2> tcs;

		// TODO: Add transformational stuff here
		glm::mat4 translate = glm::mat4{1};
		glm::mat4 scale = glm::mat4{1};
		glm::mat4 rotate = glm::mat4{1};
	};

	rect_t makeRectangle(float xMin, float yMin, float xMax, float yMax);
	circle_t make_circle(float cx, float cy, float r, int detail = 32);
} // namespace shapes

#endif	// SHAPES_HPP
