#pragma once

#include "GameScreen.h"

#include <vector>
#include <time.h>
#include <random>

constexpr int EMPTY_BLOCK = 0;
constexpr int SOLID_BLOCK = 1;
constexpr int BLUE_BLOCK = 2;
constexpr int ORANGE_BLOCK = 3;

class PlayScreen : public GameScreen
{
	enum class Direction { UP = 0, DOWN, LEFT, RIGHT };

	struct IntersectionTest
	{
		bool intersects; //Is there intersection happening
		Direction dir;
		float px, py; //penetration
	};
public:
	PlayScreen(GameState* gameState, sf::RenderWindow& window);

	// Inherited via GameScreen
	virtual void onEvent(sf::Event& event) override;
	virtual void onUpdate(float dt) override;
	virtual void draw(sf::RenderWindow& window) override;

private:
	//Circle vs AABB intersection test
	IntersectionTest ballInteresetsBlock(float bx, float by);
	//Returns the direction that is closest to what direction the ball is going
	Direction getNearestVectorDir(float tx, float ty);

private:
	GameState* gameState;

	sf::Texture paddleTexture, backgroundTexture, ballTexture, blockTexture1, blockTexture2;
	sf::Sprite paddle, background, ball, block1, block2;

	std::vector<int> blockLayout; //Loaded from a .lvl file

	sf::Vector2f ballDirection;
	float paddleSpeed = 50, ballSpeed = 50;
	int windowWidth, windowHeight;
};

