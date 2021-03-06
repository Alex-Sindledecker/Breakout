#include "MenuScreen.h"

MenuScreen::MenuScreen(GameState* gameState, sf::RenderWindow& window)
	: playButton("Play!", 0, 0, 30, sf::Color::White, sf::Color::Red), 
	  tutorialButton("How To Play", 0, 0, 30, sf::Color::White, sf::Color::Red),
	  quitButton("Quit", 0, 0, 30, sf::Color::White, sf::Color::Red),
	  gameState(gameState)
{
	playButton.setCenterPos(window.getSize().x * 0.5, window.getSize().y * 0.25);
	tutorialButton.setCenterPos(window.getSize().x * 0.5, window.getSize().y * 0.5);
	quitButton.setCenterPos(window.getSize().x * 0.5, window.getSize().y * 0.75);
}

void MenuScreen::onEvent(sf::Event& event)
{
	if (playButton.isPressed(event))
		*gameState = GameState::PLAYING;
	else if (tutorialButton.isPressed(event))
		*gameState = GameState::TUTORIAL;
	else if (quitButton.isPressed(event))
		*gameState = GameState::QUIT;
}

void MenuScreen::onUpdate(float dt)
{
}

void MenuScreen::draw(sf::RenderWindow& window)
{
	playButton.draw(window);
	tutorialButton.draw(window);
	quitButton.draw(window);
}
