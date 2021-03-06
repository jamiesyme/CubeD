#include <exception>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "single-player-screen.hpp"
#include "system-event.hpp"
#include "world.hpp"

namespace {
	static const unsigned int depthBits = 24;
}

GameSettings::GameSettings()
{
	this->fov = 60.f;
}

Game::Game()
	: window(new sf::RenderWindow(sf::VideoMode(1920, 1080),
	                              "CubeD",
	                              sf::Style::None,
	                              sf::ContextSettings(depthBits))),
	  clock(new sf::Clock()),
	  world(new World()),
	  singlePlayerScreen(new SinglePlayerScreen(this->settings, *this->world))
{
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("glew init failed");
	}
	this->window->setPosition(sf::Vector2i(100, 100));
	this->world->generate();
}

Game::~Game() = default;

void Game::mainLoop()
{
	while (this->window->isOpen())
	{
		SystemEvent systemEvent;
		while (this->window->pollEvent(systemEvent.sfEvent)) {
			switch (systemEvent.sfEvent.type) {
			case sf::Event::Closed:
				this->window->close();
				break;

			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
			case sf::Event::MouseWheelMoved:
				this->singlePlayerScreen->onEvent(systemEvent);
				break;

			default:
				break;
			}
			systemEvent = SystemEvent();
		}

		float deltaTime = this->clock->getElapsedTime().asSeconds();
		this->clock->restart();
		this->singlePlayerScreen->tick(deltaTime);

		this->window->clear();
		//this->window->pushGLStates();
		//this->window->draw(...);
		//this->window->popGLStates();
		this->singlePlayerScreen->render(*this->window);
		this->window->display();
	}
}
