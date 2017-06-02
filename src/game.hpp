#pragma once

#include <memory>

namespace sf {
	class RenderWindow;
}

class SinglePlayerScreen;
class World;

class GameSettings {
public:
	GameSettings();
	
	float fov;
};

class Game {
public:
	Game();

	~Game();

	void mainLoop();

private:
	GameSettings settings;
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<World> world;
	std::unique_ptr<SinglePlayerScreen> singlePlayerScreen;
};
