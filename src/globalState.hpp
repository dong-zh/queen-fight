#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP

#include <stdio.h>

struct GlobalState {
	GlobalState(long long now)
	:
		now(now)
	{}

	const bool DEBUG = true;
	unsigned long frameCounter = 0;
	long long now = -1;

	enum AttackDirection {
		NOT_ATTACKING,
		LEFT,
		RIGHT
	};

	bool upHeld = false;
	bool downHeld = false;
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

	/**
	 * @brief Prints a global state for debugging
	 *
	 * @param state The state to print
	 */
	void printGlobalState() {
		printf(
			"Keys held\n"
			"\tUp: %s\n"
			"\tDown: %s\n"
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
			downHeld ? "true" : "false",
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
