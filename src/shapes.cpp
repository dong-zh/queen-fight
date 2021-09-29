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

	/**
	 *
	 * A Circle is drawn as a triangle fan.
	 * The first point is the center, and every point thereafter is on the perimeter of the circle.
	 *
	 * @param cx: center's x coordinate
	 * @param cy: center's y coordinate
	 * @param r: radius
	 * @param detail: how many sides the circle approximation should have
	 * @return: a circle mesh
	 */
	circle_t make_circle(float cx, float cy, float r, int detail) {
		circle_t c;

		c.positions.emplace_back(cx, cy, 0, 1);
		c.tcs.emplace_back(0.5f, 0.5f);
		c.nVerts++;

		float slice = 2 * M_PI/detail;
		for (int i = 0; i <= detail; ++i) {
			float theta = i * slice;
			float x = cx + r * std::cos(theta);
			float y = cy + r * std::sin(theta);

			float tx = (x/r + 1) * 0.5f;
			float ty = (y/r + 1) * 0.5f;

			c.positions.emplace_back(x, y, 0, 1);
			c.tcs.emplace_back(tx, ty);
			c.nVerts++;
		}

		glGenVertexArrays(1, &c.vao);
		glGenBuffers(1, &c.vbo);

		glBindVertexArray(c.vao);
		glBindBuffer(GL_ARRAY_BUFFER, c.vbo);

		glBufferData(GL_ARRAY_BUFFER, c.nVerts * (sizeof(glm::vec4) + sizeof(glm::vec2)), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, c.nVerts * sizeof(glm::vec4), c.positions.data());
		glBufferSubData(GL_ARRAY_BUFFER, c.nVerts * sizeof(glm::vec4), c.nVerts * sizeof(glm::vec2), c.tcs.data());

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(c.nVerts * sizeof(glm::vec4)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return c;

	}
}
