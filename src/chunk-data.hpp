#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <vector>

#include "block.hpp"

class ChunkData {
public:
	static inline const glm::uvec3& getSize()
		{
			static const glm::uvec3 chunkSize = {32, 32, 32};
			return chunkSize;
		}

	static inline size_t getBlockIndex(const uint32_t blockX,
	                                   const uint32_t blockY,
	                                   const uint32_t blockZ)
		{
			const glm::uvec3 chunkSize = ChunkData::getSize();
			return (blockX * chunkSize.z + blockZ) * chunkSize.y + blockY;
		}

	ChunkData();

	BlockId getBlockId(const uint32_t blockX,
	                   const uint32_t blockY,
	                   const uint32_t blockZ) const;

	void setBlockId(const uint32_t blockX,
	                const uint32_t blockY,
	                const uint32_t blockZ,
	                const BlockId blockId);

	std::vector<BlockId> blockIds;
};
