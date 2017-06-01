#pragma once

#include <memory>

namespace sf {
	class RenderWindow;
}

class Game {
public:
	Game();

	~Game();

	void mainLoop();

private:
	std::unique_ptr<sf::RenderWindow> window;
};
