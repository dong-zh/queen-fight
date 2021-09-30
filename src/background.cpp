#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"

class Background {
public:
	Background(GLuint program, std::string texturePath): program(program) {
		chicken3421::image_t image = chicken3421::load_image(texturePath);
		GLint imageFormat = image.n_channels == 3 ? GL_RGB : GL_RGBA;
		texture = chicken3421::make_texture();
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, image.width, image.height, 0, imageFormat, GL_UNSIGNED_BYTE, image.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);    // unbind

		rectangle = shapes::makeRectangle(-1, -1, 1, 1);
	}

	void draw() {
		glUniformMatrix4fv(glGetUniformLocation(program, "transformMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4{1}));
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(rectangle.vao);
		glBindBuffer(GL_ARRAY_BUFFER, rectangle.vbo);

		glDrawArrays(GL_TRIANGLES, 0, rectangle.nVerts);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

private:
	GLuint program;
	GLuint texture;
	shapes::rect_t rectangle;
};
