#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"
#include "globalState.hpp"

class Thrasher {
public:
	Thrasher(GLuint program, GlobalState *globalState):
		program(program),
		globalState(globalState)
	{
		init();
	}

	void frameTick() {
		processState();
	}

private:
	GlobalState *globalState;
	GLuint program;


	static const unsigned HURT_HOLD_TIME = 500;
	static const unsigned SWING_MOVE_TIME = 500;
	static const unsigned DODGE_MOVE_TIME = 100;
	static const unsigned DODGE_HOLD_TIME = 500;

	const glm::vec3 DEFAULT_IDLE_POSITION = glm::vec3{0, -.5, 0};

	const float HURT_HORIZONTAL_OFFSET = .1;
	const float HURT_VERTICAL_OFFSET = .15;

	const float SWING_SPEED = .0075;
	const float DODGE_SPEED = .0075;

	static const unsigned NUM_TEXTURES = 8;
	const std::string TEXTURE_PATHS[NUM_TEXTURES] = {
		"res/img/thrasher/thrasherDodgeLeft.png",
		"res/img/thrasher/thrasherDodgeRight.png",
		"res/img/thrasher/thrasherHurtLeft.png",
		"res/img/thrasher/thrasherHurtRight.png",
		"res/img/thrasher/thrasherIdleLeft.png",
		"res/img/thrasher/thrasherIdleRight.png",
		"res/img/thrasher/thrasherPunchLeft.png",
		"res/img/thrasher/thrasherPunchRight.png",
	};

	enum sprite_t {
		SPRITE_DODGE_LEFT,
		SPRITE_DODGE_RIGHT,
		SPRITE_HURT_LEFT,
		SPRITE_HURT_RIGHT,
		SPRITE_IDLE_LEFT,
		SPRITE_IDLE_RIGHT,
		SPRITE_PUNCH_LEFT,
		SPRITE_PUNCH_RIGHT,
	};

	enum state_t {
		STATE_IDLE,

		STATE_DODGE_LEFT,
		STATE_DOGE_RIGHT,
		STATE_SWING,

		STATE_HURT_LEFT,
		STATE_HURT_RIGHT,

		// REVIEW redundant?
		STATE_DODGE_MOVE_LEFT,
		STATE_DODGE_HOLD_LEFT,
		STATE_DODGE_RETURN_LEFT,

		STATE_DODGE_MOVE_RIGHT,
		STATE_DODGE_HOLD_RIGHT,
		STATE_DODGE_RETURN_RIGHT,

		STATE_SWING_MOVE,
		STATE_SWING_HOLD,
		STATE_SWING_RETURN,
	};

	state_t lastState = STATE_IDLE;
	state_t currentState = STATE_IDLE;
	state_t nextState = STATE_IDLE;

	shapes::rect_t rectangle;
	GLuint textures[NUM_TEXTURES];

	glm::mat4 translationMatrix = glm::mat4{1};

	void init() {
		for (int i = 0; i < NUM_TEXTURES; ++i) {
			chicken3421::image_t image = chicken3421::load_image(TEXTURE_PATHS[i]);
			GLint imageFormat = image.n_channels == 3 ? GL_RGB : GL_RGBA;
			textures[i] = chicken3421::make_texture();
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, image.width, image.height, 0, imageFormat, GL_UNSIGNED_BYTE, image.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);    // unbind
		}
		rectangle = makeRectangle();
	}

	shapes::rect_t makeRectangle() {
		const float RATIO = 1.6;
		const float WIDTH = .2;
		return shapes::makeRectangle(-WIDTH, -(WIDTH * RATIO), WIDTH, WIDTH * RATIO);
	}

	void processState() {
		// Default state variables
		globalState->thrasherAttacking = GlobalState::NOT_ATTACKING;
		globalState->thrasherInvincible = true;

		// I'd probably get a 0 in COMP2511 for this mess lmao
		switch (currentState) {
		case STATE_IDLE:
			idle();
			break;
		}
	}

	// States
	void idle() {
		if (globalState->DEBUG)
			std::cout << "Thrasher in idle state\n";

		globalState->thrasherInvincible = false;

		nextState = STATE_IDLE;
		if (globalState->queenAttacking == GlobalState::LEFT) {
			nextState = STATE_HURT_RIGHT;
		} else if (globalState->queenAttacking == GlobalState::RIGHT) {
			nextState = STATE_HURT_LEFT;
		} else if (globalState->leftHeld) {
			nextState = STATE_DODGE_LEFT;
		} else if (globalState->rightHeld) {
			nextState = STATE_DODGE_HOLD_RIGHT;
		}
		// FIXME
		nextState = STATE_IDLE;
		translationMatrix = glm::translate(glm::mat4{1}, DEFAULT_IDLE_POSITION);
		draw(SPRITE_IDLE_LEFT, translationMatrix);
	}

	void draw(sprite_t sprite, glm::mat4 translationMatrix = glm::mat4{1}, glm::mat4 scaleMatrix = glm::mat4{1}, glm::mat4 rotationMatrix = glm::mat4{1}) {
		glUniformMatrix4fv(glGetUniformLocation(program, "transformMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix * rotationMatrix * scaleMatrix));
		GLuint textureToBind = textures[sprite];
		glBindTexture(GL_TEXTURE_2D, textureToBind);
		glBindVertexArray(rectangle.vao);
		glBindBuffer(GL_ARRAY_BUFFER, rectangle.vbo);

		glDrawArrays(GL_TRIANGLES, 0, rectangle.nVerts);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};
