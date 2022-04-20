#include "MenuScreen.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    GameState gameState = GameState::MENU;

    MenuScreen menuScreen(&gameState, window);

    GameScreen* activeScreen = &menuScreen;

    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        switch (gameState)
        {
        case GameState::PLAYING:
            break;
        case GameState::PAUSED:
            break;
        case GameState::MENU:
            activeScreen = &menuScreen;
            break;
        case GameState::TUTORIAL:
            break;
        case GameState::QUIT:
            break;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            activeScreen->onEvent(event);
        }

        activeScreen->onUpdate(dt);

        window.clear();

        activeScreen->draw(window);

        window.display();
    }

    return 0;
}