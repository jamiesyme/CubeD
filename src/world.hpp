#pragma once

#include <memory>
#include <vector>

class Player;

class World {
public:
	World();

	~World();

	void render();

	Player& createPlayer();

private:
	std::vector<std::unique_ptr<Player>> players;
};
