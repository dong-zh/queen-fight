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
		init();
	}

	void frameTick() {
		processState();
	}

private:
	GlobalState *globalState;
	GLuint program;

	const unsigned HURT_HOLD_TIME = 500;
	const unsigned SWING_HOLD_TIME = 500;
	const unsigned SWING_MOVE_TIME = 100;
	const unsigned HIT_HOLD_TIME = 500;

	const glm::vec3 DEFAULT_IDLE_POSITION = glm::vec3{0, .5, 0};

	const float HURT_HORIZONTAL_OFFSET = .1;
	const float HURT_VERTICAL_OFFSET = .15;

	const float HOLD_HORIZONTAL_OFFSET = .2;
	const float SWING_VERTICAL_MOVE_SPEED = .0075;
	const float SWING_HORIZONTAL_MOVE_SPEED = .0015;

	static const unsigned NUM_TEXTURES = 9;
	const std::string TEXTURE_PATHS[NUM_TEXTURES] = {
		"res/img/queen/queenHitLeft.png",
		"res/img/queen/queenHitRight.png",
		"res/img/queen/queenHurtLeft.png",
		"res/img/queen/queenHurtRight.png",
		"res/img/queen/queenIdle0.png",
		"res/img/queen/queenIdle0Right.png",
		"res/img/queen/queenIdle1.png",
		"res/img/queen/queenSwingLeft.png",
		"res/img/queen/queenSwingRight.png",
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
		SPRITE_SWING_RIGHT,
	};

	enum state_t {
		STATE_IDLE,

		STATE_SWING_LEFT_WAIT,
		STATE_SWING_LEFT_MOVE,
		STATE_HIT_LEFT_HOLD,

		STATE_SWING_RIGHT_WAIT,
		STATE_SWING_RIGHT_MOVE,
		STATE_HIT_RIGHT_HOLD,

		STATE_HURT_LEFT,
		STATE_HURT_RIGHT,
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
		const float RATIO = 1.75;
		const float WIDTH = .2;
		return shapes::makeRectangle(-WIDTH, -(WIDTH * RATIO), WIDTH, WIDTH * RATIO);
	}

	void processState() {
		// Default state variables
		globalState->queenAttacking = GlobalState::NOT_ATTACKING;
		globalState->queenInvincible = true;

		// I should really use a proper state pattern here but I don't know enough C++ to do that
		switch (currentState) {
		case STATE_IDLE:
			idle();
			break;

		case STATE_HURT_LEFT:
			hurtLeft();
			break;
		case STATE_HURT_RIGHT:
			hurtRight();
			break;

		case STATE_SWING_LEFT_WAIT:
			swingHoldLeft();
			break;
		case STATE_SWING_LEFT_MOVE:
			swingMoveLeft();
			break;
		case STATE_HIT_LEFT_HOLD:
			hitHoldLeft();
			break;

		case STATE_SWING_RIGHT_WAIT:
			swingHoldRight();
			break;
		case STATE_SWING_RIGHT_MOVE:
			swingMoveRight();
			break;
		case STATE_HIT_RIGHT_HOLD:
			hitHoldRight();
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

	// States
	void idle() {
		if (globalState->DEBUG)
			std::cout << "Queen is in idle state\n";

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

	void hurtLeft() {
		if (globalState->DEBUG)
			std::cout << "Queen is in hurt left state\n";

		const unsigned HURT_LEFT_HOLD_TIME = HURT_HOLD_TIME;
		static long long stateStartTime = -1;

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

	void hurtRight() {
		if (globalState->DEBUG)
			std::cout << "Queen is in hurt right state\n";

		const unsigned HURT_RIGHT_HOLD_TIME = HURT_HOLD_TIME;
		static long long stateStartTime = -1;

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

	void swingHoldLeft() {
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

	void swingHoldRight() {
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

	void swingMoveLeft() {
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

	void swingMoveRight() {
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

	void hitHoldLeft() {
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

	void hitHoldRight() {
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
