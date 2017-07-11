#include "chunk-data.hpp"

ChunkData::ChunkData()
	: blockIds(std::vector((ChunkData::getSize().x *
	                        ChunkData::getSize().y *
	                        ChunkData::getSize().z),
	                       BlockId::Void))
{
}

BlockId ChunkData::getBlockId(const uint32_t x,
                              const uint32_t y,
                              const uint32_t z) const
{
	return this->blockIds[ChunkData::getBlockIndex(x, y, z)];
}

void ChunkData::setBlockId(const uint32_t x,
                           const uint32_t y,
                           const uint32_t z,
                           const BlockId blockId)
{
	this->blockIds[ChunkData::getBlockIndex(x, y, z)] = blockId;
}
