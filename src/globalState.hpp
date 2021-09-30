#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP

#include <stdio.h>

struct GlobalState {
	GlobalState(long long now):	now(now) {}

	const int QUEEN_ATTACK_POWER = 5;
	const int THRASHER_ATTACK_POWER = 5;

	const bool DEBUG = false;
	unsigned long frameCounter = 0;
	long long now = -1;

	enum AttackDirection {
		NOT_ATTACKING,
		LEFT,
		RIGHT
	};

	bool gameOver = false;

	bool upHeld = false;
	bool leftHeld = false;
	bool rightHeld = false;
	// Keys for debugging
	bool oneHeld = false;
	bool twoHeld = false;
	bool threeHeld = false;
	bool fourHeld = false;

	bool queenInvincible = false;
	int queenHealth = 100;
	AttackDirection queenAttacking = NOT_ATTACKING;

	bool thrasherInvincible = false;
	int thrasherHealth = 100;
	AttackDirection thrasherAttacking = NOT_ATTACKING;

	void checkCombat() {
		if (queenAttacking != NOT_ATTACKING && !thrasherInvincible) {
			// Queen hit thrasher
			std::cout << "Queen hit Thrasher\n";
			thrasherHealth -= QUEEN_ATTACK_POWER;
			std::cout << getHealthMessage();
		}

		if (thrasherAttacking != NOT_ATTACKING && !queenInvincible) {
			// Thrasher hit Queen
			std::cout << "Thrasher hit Queen\n";
			queenHealth -= THRASHER_ATTACK_POWER;
			std::cout << getHealthMessage();
		}

		if (queenHealth <= 0 || thrasherHealth <= 0) {
			gameOver = true;
		}
	}

	std::string getHealthMessage() {
		return "Queen's HP:\t" + std::to_string(queenHealth) + "\nThrasher's HP:\t" + std::to_string(thrasherHealth) + "\n";
	}

	/**
	 * @brief Prints a global state for debugging
	 *
	 * @param state The state to print
	 */
	void printGlobalState() {
		printf(
			"Keys held\n"
			"\tUp: %s\n"
			"\tLeft: %s\n"
			"\tRight: %s\n"
			"\t1: %s\n"
			"\t2: %s\n"
			"\t3: %s\n"
			"\t4: %s\n"

			"Queen:\n"
			"\tInvincible: %s\n"
			"\tHealth: %d\n"
			"\tAttacking: %d\n"

			"Thrasher:\n"
			"\tInvincible: %s\n"
			"\tHealth: %d\n"
			"\tAttacking: %d\n",

			upHeld ? "true" : "false",
			leftHeld ? "true" : "false",
			rightHeld ? "true" : "false",
			oneHeld ? "true" : "false",
			twoHeld ? "true" : "false",
			threeHeld ? "true" : "false",
			fourHeld ? "true" : "false",


			queenInvincible ? "true" : "false",
			queenHealth,
			queenAttacking,

			thrasherInvincible ? "true" : "false",
			thrasherHealth,
			thrasherAttacking
		);
	}
};

#endif
