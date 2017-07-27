#pragma once

#include <memory>

namespace sf {
	class Event;
	class RenderWindow;
}

class DebugOverlay;
class GameSettings;
class SystemEvent;
class UserPlayerController;
class World;

class SinglePlayerScreen {
public:
	SinglePlayerScreen(GameSettings& gameSettings, World& world);

	~SinglePlayerScreen();

	void onEvent(const SystemEvent& systemEvent);

	void tick(float deltaTime);

	void render(sf::RenderWindow& window);

private:
	GameSettings& gameSettings;
	World& world;
	std::unique_ptr<UserPlayerController> userPlayerController;
	std::unique_ptr<DebugOverlay> debugOverlay;
};
