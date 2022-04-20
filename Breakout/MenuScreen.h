#pragma once

#include "GameScreen.h"
#include "TextButton.h"

class MenuScreen : public GameScreen
{
public:
	MenuScreen(GameState* gameState, sf::RenderWindow& window);

	// Inherited via GameScreen
	virtual void onEvent(sf::Event& event) override;
	virtual void onUpdate(float dt) override;
	virtual void draw(sf::RenderWindow& window) override;

private:
	TextButton playButton, tutorialButton, quitButton;
	GameState* gameState;
};

