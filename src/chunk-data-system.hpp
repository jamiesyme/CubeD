#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "block.hpp"

class ChunkData;
class World;

class ChunkDataSystem {
public:
	ChunkDataSystem(World& world);

	~ChunkDataSystem();

	BlockId getBlockId(const int64_t blockX,
	                   const int64_t blockY,
	                   const int64_t blockZ) const;

	// Same as getBlockId(), but returns a void block instead of throwing if
	// block index is out of range.
	BlockId getBlockIdSafely(const int64_t blockX,
	                         const int64_t blockY,
	                         const int64_t blockZ) const;

	void setBlockId(const int64_t blockX,
	                const int64_t blockY,
	                const int64_t blockZ,
	                const BlockId blockId);

	// Same as setBlockId(), but silently ignores request instead of throwing if
	// block index is out of range.
	void setBlockIdSafely(const int64_t blockX,
	                      const int64_t blockY,
	                      const int64_t blockZ,
	                      const BlockId blockId);

	ChunkData& getChunkData(const int32_t chunkX,
	                        const int32_t chunkY,
	                        const int32_t chunkZ);

	const ChunkData& getChunkData(const int32_t chunkX,
	                              const int32_t chunkY,
	                              const int32_t chunkZ) const;

	// Same as getChunkData(), but returns a void chunk instead of throwing if
	// chunk index is out of range.
	const ChunkData& getChunkDataSafely(const int32_t chunkX,
	                                    const int32_t chunkY,
	                                    const int32_t chunkZ) const;

	// Creates the chunk data (if it doesn't exist) and returns it.
	ChunkData& mapChunkData(const int32_t chunkX,
	                        const int32_t chunkY,
	                        const int32_t chunkZ);

	// Destroys the chunk data (if it exists).
	void unmapChunkData(const int32_t chunkX,
	                    const int32_t chunkY,
	                    const int32_t chunkZ);

private:
	bool isChunkIndexInRange(const int32_t chunkX,
	                         const int32_t chunkY,
	                         const int32_t chunkZ) const;

	size_t getChunkIndex(const int32_t chunkX,
	                     const int32_t chunkY,
	                     const int32_t chunkZ) const;

	// Grows the chunk data array to hold a particular chunk.
	void growChunkData(const int32_t chunkX,
	                   const int32_t chunkY,
	                   const int32_t chunkZ);

	// Shrinks the chunk data array as much as possible, removing null chunk
	// data pointers.
	void shrinkChunkData();

	World& world;
	std::unique_ptr<ChunkData> voidChunk;
	std::vector<std::unique_ptr<ChunkData>> chunkData;
	glm::ivec3 chunkDataOffset;
	glm::uvec3 chunkDataSize;
};
