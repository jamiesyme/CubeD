#include <SFML/OpenGL.hpp>

#include "chunk-data.hpp"
#include "player.hpp"
#include "world.hpp"

namespace {
	static void generateChunk(ChunkData& chunkData)
	{
		const glm::uvec3 chunkSize = ChunkData::getSize();
		for (uint32_t x = 0; x < chunkSize.x; ++x) {
			for (uint32_t z = 0; z < chunkSize.z; ++z) {
				for (uint32_t y = 0; y < chunkSize.y; ++y) {

					const unsigned int index = ChunkData::getBlockIndex(x, y, z);
					if (rand() % 70 == 0)
						chunkData.blockIds[index] = BlockId::Solid;
					else
						chunkData.blockIds[index] = BlockId::Empty;
				}
			}
		}
	}
}

World::World()
	: chunkDataSystem(*this),
	  chunkMeshSystem(*this)
{
}

World::~World() = default;

void World::generate()
{
	ChunkData& chunkData = this->chunkDataSystem.mapChunkData(0, 0, 0);
	generateChunk(chunkData);

	this->chunkMeshSystem.mapChunkMesh(0, 0, 0);
	this->chunkMeshSystem.updateChunkMesh(0, 0, 0, chunkData);
}

void World::render()
{
	this->chunkMeshSystem.render();
}

Player& World::createPlayer()
{
	this->players.emplace_back(new Player(*this));
	return *this->players.back();
}
