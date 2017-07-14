#include <SFML/OpenGL.hpp>

#include "chunk-data.hpp"
#include "FastNoise.h"
#include "player.hpp"
#include "world.hpp"

namespace {
	static void generateChunk(const int32_t chunkX,
	                          const int32_t chunkY,
	                          const int32_t chunkZ,
	                          ChunkData& chunkData)
	{
		const glm::uvec3 chunkSize = ChunkData::getSize();
		const int64_t blockOffsetX = chunkX * chunkSize.x;
		const int64_t blockOffsetY = chunkY * chunkSize.y;
		const int64_t blockOffsetZ = chunkZ * chunkSize.z;

		FastNoise noiseGen;
		noiseGen.SetNoiseType(FastNoise::Perlin);
		noiseGen.SetFrequency(0.015);

		for (uint32_t x = 0; x < chunkSize.x; ++x) {
			for (uint32_t z = 0; z < chunkSize.z; ++z) {

				const float noise = noiseGen.GetNoise((float)blockOffsetX + x,
				                                      (float)blockOffsetZ + z);
				const float normNoise = (noise + 1) / 2;

				const uint32_t height = (uint32_t)(normNoise * 100) + 16;
				uint32_t relHeight;
				if (height < blockOffsetY) {
					relHeight = 0;
				} else if (height >= blockOffsetY + chunkSize.y) {
					relHeight = chunkSize.y;
				} else {
					relHeight = height - blockOffsetY;
				}

				for (uint32_t y = 0; y < relHeight; ++y) {
					const size_t index = ChunkData::getBlockIndex(x, y, z);
					chunkData.blockIds[index] = BlockId::Solid;
				}
				for (uint32_t y = relHeight; y < chunkSize.y; ++y) {
					const size_t index = ChunkData::getBlockIndex(x, y, z);
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
	const glm::ivec3 size = {6, 4, 6};
	for (int32_t x = 0; x < size.x; ++x) {
		for (int32_t z = 0; z < size.z; ++z) {
			for (int32_t y = 0; y < size.y; ++y) {
				ChunkData& chunkData = this->chunkDataSystem.mapChunkData(x, y, z);
				generateChunk(x, y, z, chunkData);
			}
		}
	}
	for (int32_t x = 0; x < size.x; ++x) {
		for (int32_t z = 0; z < size.z; ++z) {
			for (int32_t y = 0; y < size.y; ++y) {
				const ChunkData& chunkData = this->chunkDataSystem.getChunkData(x, y, z);
				this->chunkMeshSystem.mapChunkMesh(x, y, z);
				this->chunkMeshSystem.updateChunkMesh(x, y, z, chunkData);
			}
		}
	}
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
