#include "PlayScreen.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

//degrees to radians
constexpr float DTR = 0.0174533f;

PlayScreen::PlayScreen(GameState* gameState, sf::RenderWindow& window)
	: gameState(gameState), windowWidth(window.getSize().x), windowHeight(window.getSize().y)
{
	backgroundTexture.loadFromFile("res/img/background.jpg");
	background.setTexture(backgroundTexture);
	background.scale(0.8, 0.8);

	paddleTexture.loadFromFile("res/img/paddle.png");
	paddle.setTexture(paddleTexture);
	paddle.setScale(0.2, 0.2);
	paddle.setPosition(window.getSize().x / 2 - paddle.getGlobalBounds().width / 2, window.getSize().y - paddle.getGlobalBounds().height - 15);

	ballTexture.loadFromFile("res/img/awesomeface.png");
	ball.setTexture(ballTexture);
	ball.setScale(0.07, 0.07);
	ball.setPosition(window.getSize().x / 2 - ball.getGlobalBounds().width / 2, window.getSize().y / 2 - ball.getGlobalBounds().height / 2);

	//Set ball to point in a random direction
	float theta = rand() % 360 * DTR;
	ballDirection.x = cos(theta);
	ballDirection.y = sin(theta);

	blockTexture1.loadFromFile("res/img/block.png");
	blockTexture2.loadFromFile("res/img/block_solid.png");
	block1.setTexture(blockTexture1);
	block2.setTexture(blockTexture2);
	block1.setScale(0.625, 0.4);
	block2.setScale(0.625, 0.4);

	block2.setColor(sf::Color(204, 204, 178));

	std::ifstream levelFile("res/levels/1.lvl");
	if (!levelFile.is_open())
		std::cout << "ERROR: Failed to open '1.lvl'!" << std::endl;
	else
	{
		std::string line;
		while (std::getline(levelFile, line))
		{
			std::stringstream ss(line);

			int blockType;
			while (ss >> blockType)
				blockLayout.push_back(blockType);
		}
	}
	std::cout << std::endl;
}

void PlayScreen::onEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		*gameState = GameState::PAUSED;
}

void PlayScreen::onUpdate(float dt)
{
	const float speedScale = 10;

	//Move paddle if its within the window bounds
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x <= windowWidth - paddle.getGlobalBounds().width)
		paddle.move(paddleSpeed * dt * speedScale, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x >= 0)
		paddle.move(-paddleSpeed * dt * speedScale, 0);

	ball.move(ballDirection.x * ballSpeed * dt * speedScale, ballDirection.y * ballSpeed * dt * speedScale);

	float bx = 0, by = 0;
	for (int i = 0; i < blockLayout.size(); i++)
	{
		if (blockLayout[i] != 0)
		{
			IntersectionTest test = ballInteresetsBlock(bx, by);
			if (test.intersects)
			{
				//Collision!
				if (blockLayout[i] != 1)
					blockLayout[i] = 0;

				switch (test.dir)
				{
				case PlayScreen::Direction::UP:
					ball.move(0, -test.py);
					ballDirection.y = -ballDirection.y;
					break;
				case PlayScreen::Direction::DOWN:
					ball.move(0, test.py);
					ballDirection.y = -ballDirection.y;
					break;
				case PlayScreen::Direction::LEFT:
					ball.move(-test.px, 0);
					ballDirection.x = -ballDirection.x;
					break;
				case PlayScreen::Direction::RIGHT:
					ball.move(test.px, 0);
					ballDirection.x = -ballDirection.x;
					break;
				default:
					break;
				}
			}
		}

		bx += block1.getGlobalBounds().width;
		if (bx >= windowWidth)
		{
			bx = 0;
			by += block1.getGlobalBounds().height;
		}
	}

	if (ball.getPosition().x < 0 || ball.getPosition().x > windowWidth - ball.getGlobalBounds().width)
		ballDirection.x = -ballDirection.x;
	if (ball.getPosition().y < 0 || ball.getPosition().y > windowHeight - ball.getGlobalBounds().height)
		ballDirection.y = -ballDirection.y;
}

void PlayScreen::draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(paddle);

	int x = 0, y = 0;
	for (int blockType : blockLayout)
	{
		block1.setPosition(x, y);
		block2.setPosition(x, y);
		switch (blockType)
		{
		case SOLID_BLOCK:
			window.draw(block2);
			break;
		case BLUE_BLOCK:
			block1.setColor(sf::Color(255 * 0.2f, 255 * 0.6f, 255 * 1.0f));
			window.draw(block1);
			break;
		case ORANGE_BLOCK:
			block1.setColor(sf::Color(255 * 1.0f, 255 * 0.5f, 255 * 0.0f));
			window.draw(block1);
			break;
		}

		x += block1.getGlobalBounds().width;
		if (x >= window.getSize().x)
		{
			x = 0;
			y += block1.getGlobalBounds().height;
		}
	}

	window.draw(ball);
}

//Circle vs AABB intersection test
PlayScreen::IntersectionTest PlayScreen::ballInteresetsBlock(float bx, float by)
{
	//Block center
	float cx = bx + block1.getGlobalBounds().width / 2.f;
	float cy = by + block1.getGlobalBounds().height / 2.f;

	//Ball center
	float x = ball.getPosition().x + ball.getGlobalBounds().width / 2.f;
	float y = ball.getPosition().y + ball.getGlobalBounds().height / 2.f;
	
	//Ball radius
	float radius = ball.getGlobalBounds().width / 2;

	//Distance between center x and y
	float dy = y - cy;
	float dx = x - cx;

	//Clamped dx and dy (the nearest point on the block to the ball)
	float cdx = std::max(-block1.getGlobalBounds().width / 2.f, std::min(block1.getGlobalBounds().width / 2.f, dx));
	float cdy = std::max(-block1.getGlobalBounds().height / 2.f, std::min(block1.getGlobalBounds().height / 2.f, dy));

	float diffx = cdx + cx - x;
	float diffy = cdy + cy - y;

	float distance = sqrt(pow(diffx, 2) + pow(diffy, 2));

	return { distance < radius, getNearestVectorDir(diffx, diffy), radius - abs(diffx), radius - abs(diffy) };
}

PlayScreen::Direction PlayScreen::getNearestVectorDir(float tx, float ty)
{
	sf::Vector2f dirVectors[] = {
		{ 0, 1 }, //up
		{ 0, -1 }, //down
		{ -1, 0 }, //left
		{ 1, 0 }, //right
	};

	float maxDot = 0;
	int bestDirIndex = -1;

	for (int i = 0; i < 4; i++)
	{
		//Dot product
		float dot = dirVectors[i].x * tx + dirVectors[i].y * ty;
		if (dot > maxDot)
		{
			maxDot = dot;
			bestDirIndex = i;
		}
	}

	return (Direction)bestDirIndex;
}
