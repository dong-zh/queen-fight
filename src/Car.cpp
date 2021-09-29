#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"
#include "userInput.hpp"

const bool DEBUG = false;

class Car {
public:
	Car(GLuint program, float x, float y, long long now) {
		this->program = program;
		this->lastTransition = now;

		// car0 texture
		chicken3421::image_t car0 = chicken3421::load_image("res/img/car0.png");
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
		chicken3421::image_t car1 = chicken3421::load_image("res/img/car1.png");
		GLint car1ImageFormat = car1.n_channels == 3 ? GL_RGB : GL_RGBA;
		std::cout << "car1 " << car1.n_channels << " channels\n";
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

	void render(long long now, UserInput keyPresses) {
		long long timeSinceLastRender = now - (lastRender == -1 ? now : lastRender);
		lastRender = now;

		if (DEBUG) {
			std::cout << "now: " << now << '\n';
			std::cout << "lr: " << lastRender << '\n';
			std::cout << "tlr: " << timeSinceLastRender << '\n';
		}

		float xTranslate = calculateXTranslate(keyPresses, timeSinceLastRender);
		float yTranslate = calculateYTranslate(keyPresses, timeSinceLastRender);

		if (DEBUG)
			std::cout << "xTranslate: " << xTranslate << ", yTranslate: " << yTranslate << '\n';


		GLuint location = glGetUniformLocation(program, "tex");

		// Texture switching logic
		glUniform1i(location, 0);
		if (now < lastTransition + ANIMATION_HOLD_TIME) {
			glBindTexture(GL_TEXTURE_2D, lastTexture ? texture1 : texture0);
		} else {
			glBindTexture(GL_TEXTURE_2D, !lastTexture ? texture1 : texture0);
			lastTexture = !lastTexture;
			lastTransition = now;


		}

		translationMatrix = glm::translate(translationMatrix, glm::vec3{xTranslate , yTranslate, 0});
		glUniformMatrix4fv(glGetUniformLocation(program, "transformMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix * rotationMatrix * scaleMatrix));

		glBindVertexArray(rectangle.vao);
		glBindBuffer(GL_ARRAY_BUFFER, rectangle.vbo);

		glDrawArrays(GL_TRIANGLES, 0, rectangle.nVerts);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

private:
	const unsigned ANIMATION_HOLD_TIME = 500;
	const float VERTICAL_SPEED = .0005;
	const float HORIZONTAL_SPEED = .0005;

	long long lastTransition;
	long long lastRender = -1;

	GLuint program;

	shapes::rect_t rectangle;
	GLuint texture0;
	GLuint texture1;

	int lastTexture = 0;

	glm::mat4 translationMatrix = glm::mat4{1};
	glm::mat4 scaleMatrix = glm::mat4{1};
	glm::mat4 rotationMatrix = glm::mat4{1};

	shapes::rect_t makeRectangle(float x, float y) {
		const float DEFAULT_WIDTH = .2;
		const float TEXTURE_RATIO = 49. / 38.;
		return shapes::makeRectangle(-DEFAULT_WIDTH, -(DEFAULT_WIDTH / TEXTURE_RATIO), DEFAULT_WIDTH, DEFAULT_WIDTH / TEXTURE_RATIO);
	}

	long calculateNextTransition() {
		return 0;
	}

	float calculateYTranslate(UserInput keyPresses, long long timeSinceLastRender) {
		float yTranslate = 0;
		if (keyPresses.upHeld) {
			yTranslate = (float)timeSinceLastRender * VERTICAL_SPEED;
		} else if (keyPresses.downHeld) {
			yTranslate = -(float)timeSinceLastRender * VERTICAL_SPEED;
		}
		return yTranslate;
	}

	float calculateXTranslate(UserInput keyPresses, long long timeSinceLastRender) {
		float xTranslate = 0;
		if (keyPresses.rightHeld) {
			xTranslate = (float)timeSinceLastRender * HORIZONTAL_SPEED;
		} else if (keyPresses.leftHeld) {
			xTranslate = -(float)timeSinceLastRender * HORIZONTAL_SPEED;
		}
		return xTranslate;
	}
};
