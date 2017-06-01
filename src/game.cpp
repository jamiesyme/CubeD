#include <SFML/Graphics.hpp>

#include "game.hpp"

Game::Game()
	: window(new sf::RenderWindow(sf::VideoMode(1920, 1080),
	                              "CubeD",
	                              sf::Style::None))
{
	this->window->setPosition(sf::Vector2i(100, 100));
}

Game::~Game()
{
}

void Game::mainLoop()
{
	sf::CircleShape circle(100.0f);
	circle.setFillColor(sf::Color::Green);

	while (this->window->isOpen())
	{
		sf::Event event;
		while (this->window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window->close();
			}
		}

		this->window->clear();
		this->window->draw(circle);
		this->window->display();
	}
}
