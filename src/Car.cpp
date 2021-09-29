#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"

class Car {
public:
	Car(GLuint program, float x, float y) {
		this->program = program;
		// car0 texture
		car0 = chicken3421::load_image("res/img/car0.png");
		GLint car0ImageFormat = car0.n_channels == 3 ? GL_RGB : GL_RGBA;
		std::cout << "car0 " << car0.n_channels << " channels\n";
		texture0 = chicken3421::make_texture();
		glBindTexture(GL_TEXTURE_2D, texture0);
		glTexImage2D(GL_TEXTURE_2D, 0, car0ImageFormat, car0.width, car0.height, 0, car0ImageFormat, GL_UNSIGNED_BYTE, car0.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);    // unbind

		// car1 texture
		car1 = chicken3421::load_image("res/img/car1.png");
		GLint car1ImageFormat = car1.n_channels == 3 ? GL_RGB : GL_RGBA;
		std::cout << "car 1" << car1.n_channels << " channels\n";
		texture1 = chicken3421::make_texture();
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexImage2D(GL_TEXTURE_2D, 0, car1ImageFormat, car1.width, car1.height, 0, car1ImageFormat, GL_UNSIGNED_BYTE, car1.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);    // unbind

		// Make the rectangle
		rectangle = makeRectangle(x, y);
	}

	void render(int tex) {
		GLuint location = glGetUniformLocation(program, "tex");
		glUniform1i(location, 0);
		if (tex == 0) {
			// car0 = chicken3421::load_image("res/img/car0.png");
			// GLint car0ImageFormat = car0.n_channels == 3 ? GL_RGB : GL_RGBA;
			// std::cout << "car0 " << car0.n_channels << " channels\n";
			// texture0 = chicken3421::make_texture();
			glBindTexture(GL_TEXTURE_2D, texture0);
			// glTexImage2D(GL_TEXTURE_2D, 0, car0ImageFormat, car0.width, car0.height, 0, car0ImageFormat, GL_UNSIGNED_BYTE, car0.data);
			// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// glGenerateMipmap(GL_TEXTURE_2D);
			// glBindTexture(GL_TEXTURE_2D, 0);    // unbind
		} else {
			// GLint car1ImageFormat = car1.n_channels == 3 ? GL_RGB : GL_RGBA;
			// std::cout << "car 1" << car1.n_channels << " channels\n";
			// texture1 = chicken3421::make_texture();
			glBindTexture(GL_TEXTURE_2D, texture1);
			// glTexImage2D(GL_TEXTURE_2D, 0, car1ImageFormat, car1.width, car1.height, 0, car1ImageFormat, GL_UNSIGNED_BYTE, car1.data);
			// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// glGenerateMipmap(GL_TEXTURE_2D);
			// glBindTexture(GL_TEXTURE_2D, 0);    // unbind
		}
		glBindVertexArray(rectangle.vao);
		glBindBuffer(GL_ARRAY_BUFFER, rectangle.vbo);

		glDrawArrays(GL_TRIANGLES, 0, rectangle.nVerts);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

private:
	GLuint program;

	chicken3421::image_t car0;
	chicken3421::image_t car1;

	// GLint location;
	shapes::rect_t rectangle;
	GLuint texture0;
	GLuint texture1;


	shapes::rect_t makeRectangle(float x, float y) {
		return shapes::makeRectangle(-.2 + x, -.2 + y, .2 + x, .2 + y);
	}
};
