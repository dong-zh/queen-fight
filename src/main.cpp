// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"
#include "Car.cpp"
namespace helpers
{
	GLuint make_texture(const chicken3421::image_t &tex_img) {
		GLint format = tex_img.n_channels == 3 ? GL_RGB : GL_RGBA; // assuming that if not 3 channels then 4

		GLuint tex;
		glGenTextures(1, &tex);

		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex_img.width, tex_img.height, 0, format, GL_UNSIGNED_BYTE, tex_img.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		return tex;
	}
} // namespace helpers



int main() {
	GLFWwindow *win = chicken3421::make_opengl_window(720, 720, "Assignment 1");
	std::cout << glGetString(GL_VERSION) << '\n';
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSwapInterval(1);

	// Make the shaders
	GLuint vertexShader = chicken3421::make_shader("res/shaders/vert.vert", GL_VERTEX_SHADER);
	GLuint fragShader = chicken3421::make_shader("res/shaders/frag.frag", GL_FRAGMENT_SHADER);

	GLuint program = chicken3421::make_program(vertexShader, fragShader);

	// car0 texture
	// chicken3421::image_t car0Image = chicken3421::load_image("res/img/car0.png");
	// auto car0ImageFormat = car0Image.n_channels == 3 ? GL_RGB : GL_RGBA;
	// std::cout << car0Image.n_channels << " channels\n";
	// GLuint car0Texture = chicken3421::make_texture();
	// glBindTexture(GL_TEXTURE_2D, car0Texture);
	// glTexImage2D(GL_TEXTURE_2D, 0, car0ImageFormat, car0Image.width, car0Image.height, 0, car0ImageFormat, GL_UNSIGNED_BYTE, car0Image.data);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glBindTexture(GL_TEXTURE_2D, 0);    // unbind
	// auto location = glGetUniformLocation(program, "car0Texture");
	//

	// auto rectange = shapes::makeRectangle(-.5, -.5, .5, .5);

	auto car0 = Car(program, 0, 0);
	auto car1 = Car(program, .2, 0);
	glUseProgram(program);

	unsigned long frameCounter = 0;
	int switcher = 0;

	while (!glfwWindowShouldClose(win)) {
		using namespace std::chrono;
		long long now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);

		// glBindTexture(GL_TEXTURE_2D, car0Texture);
		// glUniform1i(location, 0);
		// glBindVertexArray(rectange.vao);
		// glBindBuffer(GL_ARRAY_BUFFER, rectange.vbo);

		// glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		// std::cout << "Frame: " << frameCounter << '\n';
		if (frameCounter < 500) {
			// std::cout <<  "Rendered " << frameCounter << '\n';
		} else {
			switcher = !switcher;
			frameCounter = 0;
		}

		car0.render(switcher);



		// glClear(GL_COLOR_BUFFER_BIT);
		// glClearColor(std::cos(now/1000), std::sin(now/1000), std::cos(now/1000), 1);

		glfwSwapBuffers(win);
		++frameCounter;
	}


	return EXIT_SUCCESS;
}
