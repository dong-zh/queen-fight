#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shapes.hpp"
#include "globalState.hpp"

class Queen {
public:
	Queen(GLuint program, GlobalState *globalState, unsigned randomSeed = time(0)):
		program(program),
		globalState(globalState)
	{
		srand(randomSeed);
		// this->lastTransition = now;
		init();
		// // car0 texture
		// chicken3421::image_t car0 = chicken3421::load_image("res/img/car0.png");
		// GLint car0ImageFormat = car0.n_channels == 3 ? GL_RGB : GL_RGBA;
		// std::cout << "car0 " << car0.n_channels << " channels\n";
		// texture0 = chicken3421::make_texture();
		// glBindTexture(GL_TEXTURE_2D, texture0);
		// glTexImage2D(GL_TEXTURE_2D, 0, car0ImageFormat, car0.width, car0.height, 0, car0ImageFormat, GL_UNSIGNED_BYTE, car0.data);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// glGenerateMipmap(GL_TEXTURE_2D);
		// glBindTexture(GL_TEXTURE_2D, 0);    // unbind

		// // car1 texture
		// chicken3421::image_t car1 = chicken3421::load_image("res/img/car1.png");
		// GLint car1ImageFormat = car1.n_channels == 3 ? GL_RGB : GL_RGBA;
		// std::cout << "car1 " << car1.n_channels << " channels\n";
		// texture1 = chicken3421::make_texture();
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glTexImage2D(GL_TEXTURE_2D, 0, car1ImageFormat, car1.width, car1.height, 0, car1ImageFormat, GL_UNSIGNED_BYTE, car1.data);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// glGenerateMipmap(GL_TEXTURE_2D);
		// glBindTexture(GL_TEXTURE_2D, 0);    // unbind

		// // Make the rectangle
		// rectangle = makeRectangle(x, y);
	}

	void frameTick() {
		processState();
		// long long timeSinceLastRender = now - (lastRender == -1 ? now : lastRender);
		// lastRender = now;

		// if (DEBUG) {
		// 	std::cout << "now: " << now << '\n';
		// 	std::cout << "lr: " << lastRender << '\n';
		// 	std::cout << "tlr: " << timeSinceLastRender << '\n';
		// }

		// float xTranslate = calculateXTranslate(keyPresses, timeSinceLastRender);
		// float yTranslate = calculateYTranslate(keyPresses, timeSinceLastRender);

		// if (DEBUG)
		// 	std::cout << "xTranslate: " << xTranslate << ", yTranslate: " << yTranslate << '\n';


		// GLuint location = glGetUniformLocation(program, "tex");

		// // Texture switching logic
		// glUniform1i(location, 0);
		// if (now < lastTransition + ANIMATION_HOLD_TIME) {
		// 	glBindTexture(GL_TEXTURE_2D, lastTexture ? texture1 : texture0);
		// } else {
		// 	glBindTexture(GL_TEXTURE_2D, !lastTexture ? texture1 : texture0);
		// 	lastTexture = !lastTexture;
		// 	lastTransition = now;


		// }

		// translationMatrix = glm::translate(translationMatrix, glm::vec3{xTranslate , yTranslate, 0});
		// glUniformMatrix4fv(glGetUniformLocation(program, "transformMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix * rotationMatrix * scaleMatrix));

		// glBindVertexArray(rectangle.vao);
		// glBindBuffer(GL_ARRAY_BUFFER, rectangle.vbo);

		// glDrawArrays(GL_TRIANGLES, 0, rectangle.nVerts);

		// glBindTexture(GL_TEXTURE_2D, 0);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		// glBindVertexArray(0);

	}

private:
	// static const unsigned ANIMATION_HOLD_TIME = 500;
	// static const float VERTICAL_SPEED = .0005;
	// static const float HORIZONTAL_SPEED = .0005;

	GlobalState *globalState;

	static const int NUM_TEXTURES = 9;

	static const unsigned HURT_HOLD_TIME = 500;
	static const unsigned SWING_HOLD_TIME = 500;
	static const unsigned SWING_MOVE_TIME = 100;
	static const unsigned HIT_HOLD_TIME = 500;

	const glm::vec3 DEFAULT_IDLE_POSITION = glm::vec3{0, .5, 0};

	const float HURT_HORIZONTAL_OFFSET = .1;
	const float HURT_VERTICAL_OFFSET = .15;

	const float HOLD_HORIZONTAL_OFFSET = .2;
	const float SWING_VERTICAL_MOVE_SPEED = .0075;
	const float SWING_HORIZONTAL_MOVE_SPEED = .0015;

	const std::string TEXTURE_PATHS[NUM_TEXTURES] = {
		"res/img/queen/queenHitLeft.png",
		"res/img/queen/queenHitRight.png",
		"res/img/queen/queenHurtLeft.png",
		"res/img/queen/queenHurtRight.png",
		"res/img/queen/queenIdle0.png",
		"res/img/queen/queenIdle0Right.png",
		"res/img/queen/queenIdle1.png",
		"res/img/queen/queenSwingLeft.png",
		"res/img/queen/queenSwingRight.png"
	};

	enum sprite_t {
		SPRITE_HOLD_LEFT,
		SPRITE_HOLD_RIGHT,
		SPRITE_HURT_LEFT,
		SPRITE_HURT_RIGHT,
		SPRITE_IDLE0,
		SPRITE_IDLE0_RIGHT,
		SPRITE_IDLE1,
		SPRITE_SWING_LEFT,
		SPRITE_SWING_RIGHT
	};

	// I should really use a proper state pattern here but I don't know enough C++ to do that
	enum state_t {
		STATE_IDLE,
		STATE_SWING_LEFT_WAIT,
		STATE_SWING_LEFT_MOVE,
		STATE_HIT_LEFT_HOLD,
		STATE_SWING_RIGHT_WAIT,
		STATE_SWING_RIGHT_MOVE,
		STATE_HIT_RIGHT_HOLD,
		STATE_HURT_LEFT,
		STATE_HURT_RIGHT
	};

	state_t lastState = STATE_IDLE;
	state_t currentState = STATE_IDLE;
	state_t nextState = STATE_IDLE;

	// long long lastTransition;
	// long long lastRender = -1;

	GLuint program;

	// GLuint texture0;
	// GLuint texture1;

	shapes::rect_t rectangle;

	GLuint textures[NUM_TEXTURES];

	// int lastTexture = 0;

	glm::mat4 translationMatrix = glm::mat4{1};
	// glm::mat4 scaleMatrix = glm::mat4{1};
	// glm::mat4 rotationMatrix = glm::mat4{1};

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
		const float RATIO = 1.75;
		const float WIDTH = .2;
		return shapes::makeRectangle(-WIDTH, -(WIDTH * RATIO), WIDTH, WIDTH * RATIO);
	}

	void processState() {
		// Default state variables
		globalState->queenAttacking = GlobalState::NOT_ATTACKING;
		globalState->queenInvincible = true;


		switch (currentState) {
		case STATE_IDLE:
			idleState();
			break;

		case STATE_HURT_LEFT:
			hurtLeftState();
			break;
		case STATE_HURT_RIGHT:
			hurtRightState();
			break;

		case STATE_SWING_LEFT_WAIT:
			swingLeftHoldState();
			break;
		case STATE_SWING_LEFT_MOVE:
			swingLeftMoveState();
			break;
		case STATE_HIT_LEFT_HOLD:
			hitLeftHoldState();
			break;

		case STATE_SWING_RIGHT_WAIT:
			swingRightHoldState();
			break;
		case STATE_SWING_RIGHT_MOVE:
			swingRightMoveState();
			break;
		case STATE_HIT_RIGHT_HOLD:
			hitRightHoldState();
			break;

		default:
			if (globalState->DEBUG)
				std::cout << "INVALID STATE\n";
		}


		lastState = currentState;
		currentState = nextState;
	}

	GlobalState::AttackDirection shouldAttack() {
		const int ATTACK_CHANCE = 20;
		const unsigned TIME_BETWEEN_ATTACK_CHECK = 500;

		static long long lastAttackCalculation = globalState->now;

		// If time to attack and random generator says attack
		if (globalState->now > lastAttackCalculation + TIME_BETWEEN_ATTACK_CHECK && rand() % 100 < ATTACK_CHANCE) {
			if (rand() % 100 < ATTACK_CHANCE) {
				// Attack now
				// 50 50 chance of attack left or attacking right
				return rand() % 100 < 50 ? GlobalState::LEFT : GlobalState::RIGHT;
			}
			lastAttackCalculation = globalState->now;
		}

		return GlobalState::NOT_ATTACKING;
	}

	void idleState() {
		if (globalState->DEBUG)
			std::cout << "Queen in idle state\n";

		const unsigned IDLE_TEXTURE_HOLD_TIME = 500;

		static long long lastSpriteSwitch = globalState->now;
		static sprite_t currentSprite = SPRITE_IDLE1;

		// Queen isn't invincible when idle
		globalState->queenInvincible = false;

		if (globalState->now > lastSpriteSwitch + IDLE_TEXTURE_HOLD_TIME) {
			// Should switch idle sprites
			currentSprite = currentSprite == SPRITE_IDLE1 ? SPRITE_IDLE0 : SPRITE_IDLE1;
			lastSpriteSwitch = globalState->now;
		}

		translationMatrix = glm::translate(glm::mat4{1}, DEFAULT_IDLE_POSITION);
		draw(currentSprite, translationMatrix);

		// State switch logic
		nextState = STATE_IDLE;
		GlobalState::AttackDirection attackDirection = shouldAttack();
		if (globalState->thrasherAttacking == GlobalState::LEFT) {
			nextState = STATE_HURT_LEFT;
		} else if (globalState->thrasherAttacking == GlobalState::RIGHT) {
			nextState = STATE_HURT_RIGHT;
		} else if (attackDirection == GlobalState::LEFT) {
			nextState = STATE_SWING_LEFT_WAIT;
		} else if (attackDirection == GlobalState::RIGHT) {
			nextState = STATE_SWING_RIGHT_WAIT;
		}
	}

	void hurtLeftState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in hurt left state\n";

		const unsigned HURT_LEFT_HOLD_TIME = HURT_HOLD_TIME;
		static long long stateStartTime = -1;

		globalState->queenInvincible = false;


		// State transition logic
		nextState = STATE_HURT_LEFT;
		GlobalState::AttackDirection attackDirection = shouldAttack();
		if (attackDirection == GlobalState::LEFT) {
			nextState = STATE_SWING_LEFT_WAIT;
		} else if (attackDirection == GlobalState::RIGHT) {
			nextState = STATE_SWING_RIGHT_WAIT;
		} else if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			translationMatrix = glm::mat4{1};
			translationMatrix = glm::translate(translationMatrix, DEFAULT_IDLE_POSITION);
			translationMatrix = glm::translate(translationMatrix, glm::vec3{-HURT_HORIZONTAL_OFFSET, HURT_VERTICAL_OFFSET, 0});
		} else {
			// Already holding
			if (globalState->now > stateStartTime + HURT_LEFT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_IDLE;
			}
		}

		draw(SPRITE_HURT_LEFT, translationMatrix);
	}

	void hurtRightState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in hurt right state\n";

		const unsigned HURT_RIGHT_HOLD_TIME = HURT_HOLD_TIME;
		static long long stateStartTime = -1;

		globalState->queenInvincible = false;


		// State transition logic
		nextState = STATE_HURT_RIGHT;
		GlobalState::AttackDirection attackDirection = shouldAttack();
		if (attackDirection == GlobalState::RIGHT) {
			nextState = STATE_SWING_RIGHT_WAIT;
		} else if (attackDirection == GlobalState::RIGHT) {
			nextState = STATE_SWING_RIGHT_WAIT;
		} else if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			translationMatrix = glm::mat4{1};
			translationMatrix = glm::translate(translationMatrix, DEFAULT_IDLE_POSITION);
			translationMatrix = glm::translate(translationMatrix, glm::vec3{HURT_HORIZONTAL_OFFSET, HURT_VERTICAL_OFFSET, 0});
		} else {
			// Already holding
			if (globalState->now > stateStartTime + HURT_RIGHT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_IDLE;
			}
		}

		draw(SPRITE_HURT_RIGHT, translationMatrix);
	}

	void swingLeftHoldState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in swing left hold state\n";

		const unsigned SWING_LEFT_HOLD_TIME = SWING_HOLD_TIME;
		static long long stateStartTime = globalState->now;

		// State transition logic
		nextState = STATE_SWING_LEFT_WAIT;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			translationMatrix = glm::mat4{1};
			translationMatrix = glm::translate(translationMatrix, DEFAULT_IDLE_POSITION);
			translationMatrix = glm::translate(translationMatrix, glm::vec3{-HOLD_HORIZONTAL_OFFSET, 0, 0});
		} else {
			// Already holding
			if (globalState->now > stateStartTime + SWING_LEFT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_SWING_LEFT_MOVE;
			}
		}
		draw(SPRITE_IDLE0, translationMatrix);
	}

	void swingRightHoldState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in swing right hold state\n";

		const unsigned SWING_RIGHT_HOLD_TIME = SWING_HOLD_TIME;
		static long long stateStartTime = globalState->now;

		// State transition logic
		nextState = STATE_SWING_RIGHT_WAIT;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			translationMatrix = glm::mat4{1};
			translationMatrix = glm::translate(translationMatrix, DEFAULT_IDLE_POSITION);
			translationMatrix = glm::translate(translationMatrix, glm::vec3{HOLD_HORIZONTAL_OFFSET, 0, 0});
		} else {
			// Already holding
			if (globalState->now > stateStartTime + SWING_RIGHT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_SWING_RIGHT_MOVE;
			}
		}
		draw(SPRITE_IDLE0_RIGHT, translationMatrix);
	}

	void swingLeftMoveState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in left move state\n";

		const unsigned SWING_LEFT_HOLD_TIME = SWING_MOVE_TIME;

		static long long stateStartTime = globalState->now;
		static long long lastMovement = globalState->now;
		// static glm::mat4 translationMatrix = glm::mat4{1};

		// State transition logic
		nextState = STATE_SWING_LEFT_MOVE;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			lastMovement = globalState->now;
			// translationMatrix = glm::mat4{1};
		} else {
			// Already holding
			if (globalState->now > stateStartTime + SWING_LEFT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_HIT_LEFT_HOLD;
			}
		}
		float verticalMoveAmount = (globalState->now - lastMovement) * -SWING_VERTICAL_MOVE_SPEED;
		float horizontalMoveAmount = (globalState->now - lastMovement) * SWING_HORIZONTAL_MOVE_SPEED;
		translationMatrix = glm::translate(translationMatrix, glm::vec3{horizontalMoveAmount, verticalMoveAmount, 0});

		draw(SPRITE_SWING_LEFT, translationMatrix);
		lastMovement = globalState->now;
	}

	void swingRightMoveState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in right move state\n";

		const unsigned SWING_RIGHT_HOLD_TIME = SWING_MOVE_TIME;

		static long long stateStartTime = globalState->now;
		static long long lastMovement = globalState->now;
		// static glm::mat4 translationMatrix = glm::mat4{1};

		// State transition logic
		nextState = STATE_SWING_RIGHT_MOVE;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
			lastMovement = globalState->now;
			// translationMatrix = glm::mat4{1};
		} else {
			// Already holding
			if (globalState->now > stateStartTime + SWING_RIGHT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_HIT_RIGHT_HOLD;
			}
		}
		float verticalMoveAmount = (globalState->now - lastMovement) * -SWING_VERTICAL_MOVE_SPEED;
		float horizontalMoveAmount = (globalState->now - lastMovement) * -SWING_HORIZONTAL_MOVE_SPEED;
		translationMatrix = glm::translate(translationMatrix, glm::vec3{horizontalMoveAmount, verticalMoveAmount, 0});

		draw(SPRITE_SWING_RIGHT, translationMatrix);
		lastMovement = globalState->now;
	}

	void hitLeftHoldState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in left hit state\n";

		const unsigned HIT_LEFT_HOLD_TIME = HIT_HOLD_TIME;

		static long long stateStartTime = globalState->now;

		globalState->queenAttacking = GlobalState::LEFT;

		// State transition logic
		nextState = STATE_HIT_LEFT_HOLD;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
		} else {
			// Already holding
			if (globalState->now > stateStartTime + HIT_LEFT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_IDLE;
			}
		}

		draw(SPRITE_SWING_LEFT, translationMatrix);
	}

	void hitRightHoldState() {
		if (globalState->DEBUG)
			std::cout << "Queen is in right hit state\n";

		const unsigned HIT_RIGHT_HOLD_TIME = HIT_HOLD_TIME;

		static long long stateStartTime = globalState->now;

		globalState->queenAttacking = GlobalState::RIGHT;

		// State transition logic
		nextState = STATE_HIT_RIGHT_HOLD;
		if (currentState != lastState) {
			// Starting hold
			stateStartTime = globalState->now;
		} else {
			// Already holding
			if (globalState->now > stateStartTime + HIT_RIGHT_HOLD_TIME) {
				// Finished holding
				nextState = STATE_IDLE;
			}
		}

		draw(SPRITE_SWING_RIGHT, translationMatrix);
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
