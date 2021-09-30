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
#include "queen.cpp"
#include "globalState.hpp"

const bool DEBUG = true;
// Global variable for key presses
GlobalState *globalState;

void keyPressHandler(GLFWwindow *window, int key, int scancode, int action, int mods);

int main() {
	// General configuration
	stbi_set_flip_vertically_on_load(true);
	GLFWwindow *win = chicken3421::make_opengl_window(720, 720, "NUTDEALER");
	std::cout << glGetString(GL_VERSION) << '\n';
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Makes the shaders and the program
	GLuint vertexShader = chicken3421::make_shader("res/shaders/vert.vert", GL_VERTEX_SHADER);
	GLuint fragShader = chicken3421::make_shader("res/shaders/frag.frag", GL_FRAGMENT_SHADER);
	GLuint program = chicken3421::make_program(vertexShader, fragShader);

	unsigned long frameCounter = 0;
	using namespace std::chrono;
	long long now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

	glfwSetKeyCallback(win, keyPressHandler);

	glUseProgram(program);

	globalState = new GlobalState(now);

	auto queen = Queen(program, globalState);
	while (!glfwWindowShouldClose(win)) {
		globalState->now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
		glfwPollEvents();

		// Set background
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, .5, 1);

		queen.frameTick();

		if (globalState->DEBUG) {
			globalState->printGlobalState();
		}

		++frameCounter;
		glfwSwapBuffers(win);
	}



	return EXIT_SUCCESS;
}

void processLogic(long long now) {

}

void keyPressHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
	bool keyPressed;
	// Only handle key pressed and key released
	if (action == GLFW_PRESS) {
		keyPressed = true;
	} else if (action == GLFW_RELEASE) {
		keyPressed = false;
	} else {
		return;
	}

	switch (key) {
	case GLFW_KEY_UP:
		globalState->upHeld = keyPressed;
		break;
	case GLFW_KEY_DOWN:
		globalState->downHeld = keyPressed;
		break;
	case GLFW_KEY_RIGHT:
		globalState->rightHeld = keyPressed;
		break;
	case GLFW_KEY_LEFT:
		globalState->leftHeld = keyPressed;
		break;
	case GLFW_KEY_1:
		globalState->oneHeld = keyPressed;
		globalState->thrasherAttacking = keyPressed ? GlobalState::LEFT : GlobalState::NOT_ATTACKING;
		break;
	case GLFW_KEY_2:
		globalState->twoHeld = keyPressed;
		globalState->thrasherAttacking = keyPressed ? GlobalState::RIGHT : GlobalState::NOT_ATTACKING;
		break;
	case GLFW_KEY_3:
		globalState->threeHeld = keyPressed;
		break;
	case GLFW_KEY_4:
		globalState->fourHeld = keyPressed;
		break;
	}
}
