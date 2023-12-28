#ifndef GLOBAL_STATE_CPP
#define GLOBAL_STATE_CPP

#include <string>
#include <iostream>

/**
 * @brief The state of the world
 */
struct GlobalState {
/**
 * @brief Creates a world state
 *
 * @param now The current time in milliseconds
 */
	GlobalState(long long now):	now(now) {}

	// Attack power
	const int QUEEN_ATTACK_POWER = 5;
	const int THRASHER_ATTACK_POWER = 5;

	// Enable debug for lots of prints
	const bool DEBUG = false;

	// Counts how many frames it's been since start
	unsigned long frameCounter = 0;

	// The current time of this particular state
	long long now = -1;

	// The attacking direction looking from Thrasher
	enum AttackDirection {
		NOT_ATTACKING,
		LEFT,
		RIGHT
	};

	// Game ends when this is set to true
	bool gameOver = false;

	// Whether the keys are held down at this particular time
	bool upHeld = false;
	bool leftHeld = false;
	bool rightHeld = false;
	// Keys for debugging
	bool oneHeld = false;
	bool twoHeld = false;
	bool threeHeld = false;
	bool fourHeld = false;

	// Combat variables for Queen
	bool queenInvincible = false;
	int queenHealth = 100;
	AttackDirection queenAttacking = NOT_ATTACKING;

	// Combat variables for Thrasher
	bool thrasherInvincible = false;
	int thrasherHealth = 100;
	AttackDirection thrasherAttacking = NOT_ATTACKING;

	/**
	 * @brief Checks whether combat is occuring and deducts HP accordingly. Also checks for game over condition
	 */
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

	/**
	 * @brief Gets a formatted printout of Queen's and Thrasher's health
	 *
	 * @return The formatted message
	 */
	std::string getHealthMessage() {
		return "Queen's HP:\t" + std::to_string(queenHealth) + "\nThrasher's HP:\t" + std::to_string(thrasherHealth) + "\n";
	}

	/**
	 * @brief Prints the global state for debugging
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
