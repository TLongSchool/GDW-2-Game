#pragma once

class PlayerStats
{
public:
	//Set up basic stats and default values to fall back on; defaults to apple stats
	int health = 3;
	int lives = 3; //Lives start at the same value for all characters
	float speed = 350000; 
	bool isFrozen = false;
	bool playerWon = false;
	bool playerLost = false;

	//Bools to determine character type and thus stats/ability.
	bool isApple = false;
	bool isBanana = false;
	bool isWaterMelon = false;

	//Checkpoint position data
	b2Vec2 checkPointPos;
};
