#pragma once

#include <memory>
#include <vector>

#include "chunk-data-system.hpp"
#include "chunk-mesh-system.hpp"

class Player;

class World {
public:
	World();

	~World();

	void generate();

	void render();

	Player& createPlayer();

	ChunkDataSystem chunkDataSystem;
	ChunkMeshSystem chunkMeshSystem;
	std::vector<std::unique_ptr<Player>> players;
};
