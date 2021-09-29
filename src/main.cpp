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
#include <stb/stb_image.h>

#include "shapes.hpp"
#include "Car.cpp"
#include "userInput.hpp"

// Global variable for key presses
UserInput keyPresses;

void keyPressHandler(GLFWwindow *window, int key, int scancode, int action, int mods);

int main() {
	stbi_set_flip_vertically_on_load(true);
	GLFWwindow *win = chicken3421::make_opengl_window(720, 720, "Assignment 1");
	std::cout << glGetString(GL_VERSION) << '\n';
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Make the shaders
	GLuint vertexShader = chicken3421::make_shader("res/shaders/vert.vert", GL_VERTEX_SHADER);
	GLuint fragShader = chicken3421::make_shader("res/shaders/frag.frag", GL_FRAGMENT_SHADER);

	GLuint program = chicken3421::make_program(vertexShader, fragShader);


	using namespace std::chrono;
	long long now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

	glfwSetKeyCallback(win, keyPressHandler);

	auto car0 = Car(program, 0, 0, now);
	auto car1 = Car(program, .2, 0, now);
	glUseProgram(program);

	while (!glfwWindowShouldClose(win)) {
		now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		// car1.render(0);
		car0.render(now, keyPresses);

		glfwSwapBuffers(win);
	}



	return EXIT_SUCCESS;
}

void processLogic(long long now) {

}

void keyPressHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
	bool keyHeld;
	// Only handle key pressed and key released
	if (action == GLFW_PRESS) {
		keyHeld = true;
	} else if (action == GLFW_RELEASE) {
		keyHeld = false;
	} else {
		return;
	}

	switch (key) {
		case GLFW_KEY_UP:
			keyPresses.upHeld = keyHeld;
			break;
		case GLFW_KEY_DOWN:
			keyPresses.downHeld = keyHeld;
			break;
		case GLFW_KEY_RIGHT:
			keyPresses.rightHeld = keyHeld;
			break;
		case GLFW_KEY_LEFT:
			keyPresses.leftHeld = keyHeld;
			break;
	}
}
