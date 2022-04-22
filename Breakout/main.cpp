#include "MenuScreen.h"
#include "PlayScreen.h"

int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);

    GameState gameState = GameState::MENU;

    MenuScreen menuScreen(&gameState, window);
    PlayScreen playScreen(&gameState, window);

    GameScreen* activeScreen = &menuScreen;

    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        switch (gameState)
        {
        case GameState::PLAYING:
            activeScreen = &playScreen;
            break;
        case GameState::PAUSED:
            activeScreen = &menuScreen;
            break;
        case GameState::MENU:
            activeScreen = &menuScreen;
            break;
        case GameState::TUTORIAL:
            break;
        case GameState::QUIT:
            window.close();
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