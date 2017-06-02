#pragma once

#include <memory>

namespace sf {
	class Event;
	class Window;
}

class GameSettings;
class UserPlayerController;
class World;

class SinglePlayerScreen {
public:
	SinglePlayerScreen(GameSettings& gameSettings, World& world);

	~SinglePlayerScreen();

	void onEvent(const sf::Event& event);

	void render(sf::Window& window);

private:
	GameSettings& gameSettings;
	World& world;
	std::unique_ptr<UserPlayerController> userPlayerController;
};
