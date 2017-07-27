#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "chunk-mesh.hpp"

class ChunkData;
class World;

class ChunkMeshSystem {
public:
	ChunkMeshSystem(World& world);

	void render() const;

	void mapChunkMesh(const int32_t chunkX,
	                  const int32_t chunkY,
	                  const int32_t chunkZ);

	void unmapChunkMesh(const int32_t chunkX,
	                    const int32_t chunkY,
	                    const int32_t chunkZ);

	void updateChunkMesh(const int32_t chunkX,
	                     const int32_t chunkY,
	                     const int32_t chunkZ,
	                     const ChunkData& chunkData);

	uint32_t getTriangleCount() const;

private:
	class ChunkMeshWithIndex : public ChunkMesh {
	public:
		ChunkMeshWithIndex(const int32_t chunkX,
		                   const int32_t chunkY,
		                   const int32_t chunkZ);

		int32_t chunkX;
		int32_t chunkY;
		int32_t chunkZ;
	};

	void updateChunkMesh(const ChunkData& chunkData,
	                     ChunkMeshWithIndex& chunkMesh);

	World& world;
	std::vector<std::unique_ptr<ChunkMeshWithIndex>> chunkMeshes;
};
