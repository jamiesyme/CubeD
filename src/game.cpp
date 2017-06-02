#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "single-player-screen.hpp"
#include "world.hpp"

GameSettings::GameSettings()
{
	this->fov = 60.f;
}

Game::Game()
	: window(new sf::RenderWindow(sf::VideoMode(1920, 1080),
	                              "CubeD",
	                              sf::Style::None)),
	  world(new World()),
	  singlePlayerScreen(new SinglePlayerScreen(this->settings, *this->world))
{
	this->window->setPosition(sf::Vector2i(100, 100));
}

Game::~Game() = default;

void Game::mainLoop()
{
	while (this->window->isOpen())
	{
		sf::Event event;
		while (this->window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				this->window->close();
				break;

			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
			case sf::Event::MouseWheelMoved:
				this->singlePlayerScreen->onEvent(event);
				break;

			default:
				break;
			}
		}

		this->window->clear();
		//this->window->pushGLStates();
		//this->window->draw(...);
		//this->window->popGLStates();
		this->singlePlayerScreen->render(*this->window);
		this->window->display();
	}
}
