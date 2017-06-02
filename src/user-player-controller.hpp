#pragma once

namespace sf {
	class Event;
}

class Player;

class UserPlayerController {
public:
	UserPlayerController(Player& player);

	void onEvent(const sf::Event& event);

	Player& player;
};
