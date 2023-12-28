#include "shapes.hpp"

namespace shapes {

	// TODO: Modify this function so that texture coordinates are made with the rest of the mesh
	rect_t makeRectangle(float xMin, float yMin, float xMax, float yMax) {
		std::vector<vertex_t> verts = {
			// {{x, y, z, a}, {texX, texY}}
			{{ xMax, yMax, 0, 1 }, {1, 1}}, // top-right
			{{ xMax, yMin, 0, 1 }, {1, 0}}, // bottom-right
			{{ xMin, yMin, 0, 1 }, {0, 0}}, // bottom-left

			{{ xMax, yMax, 0, 1 }, {1, 1}}, // top-right
			{{ xMin, yMin, 0, 1 }, {0, 0}}, // bottom-left
			{{ xMin, yMax, 0, 1 }, {0, 1}}, // top-left
		};

		GLuint vao;
		glGenVertexArrays(1, &vao);

		GLuint vbo;
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, (GLintptr)(sizeof(vertex_t) * verts.size()), verts.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(0 + offsetof(vertex_t, position)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(0 + offsetof(vertex_t, textureCoordinates)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		return rect_t{vao, vbo, verts};
	}
}
