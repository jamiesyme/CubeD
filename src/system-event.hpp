#pragma once

#include <SFML/Window/Event.hpp>

class SystemEvent {
public:
	SystemEvent();

	sf::Event sfEvent;
	bool consumed;
};
