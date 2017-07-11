#include <exception>

#include "chunk-data.hpp"
#include "chunk-data-system.hpp"
#include "world.hpp"

namespace {
	static int intFloor(const float x)
	{
		// Stolen from stack overflow:
		// https://stackoverflow.com/a/15341154/1422864
		int i = (int)x;
		return i - (i > x);
	}

	static size_t getChunkIndex(const int32_t chunkX,
	                            const int32_t chunkY,
	                            const int32_t chunkZ,
	                            const glm::ivec3& chunkDataOffset,
	                            const glm::uvec3& chunkDataSize)
	{
		return (((chunkX - chunkDataOffset.x) * chunkDataSize.z +
		         (chunkZ - chunkDataOffset.z)) * chunkDataSize.y +
		        (chunkY - chunkDataOffset.y));
	}

	static void growChunkDataBounds(const int32_t chunkX,
	                                const int32_t chunkY,
	                                const int32_t chunkZ,
	                                glm::ivec3& chunkDataOffset,
	                                glm::uvec3& chunkDataSize)
	{
		if (chunkX < chunkDataOffset.x) {
			chunkDataOffset.x = chunkX;
		}
		if (chunkY < chunkDataOffset.y) {
			chunkDataOffset.y = chunkY;
		}
		if (chunkZ < chunkDataOffset.z) {
			chunkDataOffset.z = chunkZ;
		}
		if (chunkX >= chunkDataOffset.x + (int)chunkDataSize.x) {
			chunkDataSize.x = chunkX - chunkDataOffset.x + 1;
		}
		if (chunkY >= chunkDataOffset.y + (int)chunkDataSize.y) {
			chunkDataSize.y = chunkY - chunkDataOffset.y + 1;
		}
		if (chunkZ >= chunkDataOffset.z + (int)chunkDataSize.z) {
			chunkDataSize.z = chunkZ - chunkDataOffset.z + 1;
		}
	}
}

ChunkDataSystem::ChunkDataSystem(World& world)
	: world(world),
	  voidChunk(new ChunkData())
{
	const glm::uvec3 chunkSize = ChunkData::getSize();
	for (uint32_t x = 0; x < chunkSize.x; ++x) {
		for (uint32_t z = 0; z < chunkSize.z; ++z) {
			for (uint32_t y = 0; y < chunkSize.y; ++y) {
				voidChunk->setBlockId(x, y, z, BlockId::Void);
			}
		}
	}
}

ChunkDataSystem::~ChunkDataSystem() = default;

BlockId ChunkDataSystem::getBlockId(const int64_t blockX,
                                    const int64_t blockY,
                                    const int64_t blockZ) const
{
	const glm::uvec3 chunkSize = ChunkData::getSize();
	const int32_t chunkX = intFloor((float)blockX / chunkSize.x);
	const int32_t chunkY = intFloor((float)blockY / chunkSize.y);
	const int32_t chunkZ = intFloor((float)blockZ / chunkSize.z);

	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		throw std::runtime_error("block out of range");
	}

	const ChunkData& chunkData = this->getChunkData(chunkX, chunkY, chunkZ);
	return chunkData.getBlockId(blockX - chunkX * chunkSize.x,
	                            blockY - chunkY * chunkSize.y,
	                            blockZ - chunkZ * chunkSize.z);
}

BlockId ChunkDataSystem::getBlockIdSafely(const int64_t blockX,
                                          const int64_t blockY,
                                          const int64_t blockZ) const
{
	const glm::uvec3 chunkSize = ChunkData::getSize();
	const int32_t chunkX = intFloor((float)blockX / chunkSize.x);
	const int32_t chunkY = intFloor((float)blockY / chunkSize.y);
	const int32_t chunkZ = intFloor((float)blockZ / chunkSize.z);

	const ChunkData& chunkData = this->getChunkDataSafely(chunkX, chunkY, chunkZ);
	return chunkData.getBlockId(blockX - chunkX * chunkSize.x,
	                            blockY - chunkY * chunkSize.y,
	                            blockZ - chunkZ * chunkSize.z);
}

void ChunkDataSystem::setBlockId(const int64_t blockX,
                                 const int64_t blockY,
                                 const int64_t blockZ,
                                 const BlockId blockId)
{
	const glm::uvec3 chunkSize = ChunkData::getSize();
	const int32_t chunkX = intFloor((float)blockX / chunkSize.x);
	const int32_t chunkY = intFloor((float)blockY / chunkSize.y);
	const int32_t chunkZ = intFloor((float)blockZ / chunkSize.z);

	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		throw std::runtime_error("block out of range");
	}

	ChunkData& chunkData = this->getChunkData(chunkX, chunkY, chunkZ);
	chunkData.setBlockId(blockX - chunkX * chunkSize.x,
	                     blockY - chunkY * chunkSize.y,
	                     blockZ - chunkZ * chunkSize.z,
	                     blockId);
}

ChunkData& ChunkDataSystem::getChunkData(const int32_t chunkX,
                                         const int32_t chunkY,
                                         const int32_t chunkZ)
{
	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		throw std::runtime_error("chunk out of range");
	}

	const size_t chunkIndex = this->getChunkIndex(chunkX, chunkY, chunkZ);
	ChunkData* chunkData = this->chunkData[chunkIndex].get();
	if (chunkData == NULL) {
		throw std::runtime_error("chunk out of range");
	}
	return *chunkData;
}

const ChunkData& ChunkDataSystem::getChunkData(const int32_t chunkX,
                                               const int32_t chunkY,
                                               const int32_t chunkZ) const
{
	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		throw std::runtime_error("chunk out of range");
	}

	const size_t chunkIndex = this->getChunkIndex(chunkX, chunkY, chunkZ);
	const ChunkData* chunkData = this->chunkData[chunkIndex].get();
	if (chunkData == NULL) {
		throw std::runtime_error("chunk out of range");
	}
	return *chunkData;
}

const ChunkData& ChunkDataSystem::getChunkDataSafely(const int32_t chunkX,
                                                     const int32_t chunkY,
                                                     const int32_t chunkZ) const
{
	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		return *this->voidChunk;
	}

	const size_t chunkIndex = this->getChunkIndex(chunkX, chunkY, chunkZ);
	const ChunkData* chunkData = this->chunkData[chunkIndex].get();
	if (chunkData == NULL) {
		return *this->voidChunk;
	}
	return *chunkData;
}

ChunkData& ChunkDataSystem::mapChunkData(const int32_t chunkX,
                                         const int32_t chunkY,
                                         const int32_t chunkZ)
{
	// Make sure the chunk data array is large enough
	this->growChunkData(chunkX, chunkY, chunkZ);

	// Create the chunk if needed
	const size_t chunkIndex = this->getChunkIndex(chunkX, chunkY, chunkZ);
	if (!this->chunkData[chunkIndex]) {
		this->chunkData[chunkIndex] = std::make_unique<ChunkData>();
	}

	return *this->chunkData[chunkIndex];
}

void ChunkDataSystem::unmapChunkData(const int32_t chunkX,
                                     const int32_t chunkY,
                                     const int32_t chunkZ)
{
	// If the chunk is already out of range, we're good
	if (!this->isChunkIndexInRange(chunkX, chunkY, chunkZ)) {
		return;
	}

	// If the chunk is already unmapped, we're good
	const size_t chunkIndex = this->getChunkIndex(chunkX, chunkY, chunkZ);
	if (!this->chunkData[chunkIndex]) {
		return;
	}

	// Release the chunk data
	this->chunkData[chunkIndex].release();

	// If the chunk that we just released was on the edge of our chunk data
	// array, we may be able to shrink the array
	if (chunkX == this->chunkDataOffset.x ||
	    chunkY == this->chunkDataOffset.y ||
	    chunkZ == this->chunkDataOffset.z ||
	    chunkX == this->chunkDataOffset.x + (int)this->chunkDataSize.x - 1 ||
	    chunkY == this->chunkDataOffset.y + (int)this->chunkDataSize.y - 1 ||
	    chunkZ == this->chunkDataOffset.z + (int)this->chunkDataSize.z - 1) {
		this->shrinkChunkData();
	}
}

bool ChunkDataSystem::isChunkIndexInRange(const int32_t chunkX,
                                          const int32_t chunkY,
                                          const int32_t chunkZ) const
{
	return (chunkX >= this->chunkDataOffset.x &&
	        chunkY >= this->chunkDataOffset.y &&
	        chunkZ >= this->chunkDataOffset.z &&
	        chunkX < this->chunkDataOffset.x + (int)this->chunkDataSize.x &&
	        chunkY < this->chunkDataOffset.y + (int)this->chunkDataSize.y &&
	        chunkZ < this->chunkDataOffset.z + (int)this->chunkDataSize.z);
}

size_t ChunkDataSystem::getChunkIndex(const int32_t chunkX,
                                      const int32_t chunkY,
                                      const int32_t chunkZ) const
{
	return ::getChunkIndex(chunkX,
	                       chunkY,
	                       chunkZ,
	                       this->chunkDataOffset,
	                       this->chunkDataSize);
}

void ChunkDataSystem::growChunkData(const int32_t chunkX,
                                    const int32_t chunkY,
                                    const int32_t chunkZ)
{
	// Calculate the new chunk data bounds to accommodate this new chunk
	glm::ivec3 newChunkDataOffset = this->chunkDataOffset;
	glm::uvec3 newChunkDataSize = this->chunkDataSize;
	growChunkDataBounds(chunkX,
	                    chunkY,
	                    chunkZ,
	                    newChunkDataOffset,
	                    newChunkDataSize);

	// If the bounds didn't change, we're good
	if (newChunkDataOffset == this->chunkDataOffset &&
	    newChunkDataSize == this->chunkDataSize) {
		return;
	}

	// Resize our chunk data array
	std::vector<std::unique_ptr<ChunkData>> newChunkData;
	newChunkData.resize(newChunkDataSize.x *
	                    newChunkDataSize.y *
	                    newChunkDataSize.z);

	for (uint32_t x = 0; x < this->chunkDataSize.x; ++x) {
		for (uint32_t z = 0; z < this->chunkDataSize.z; ++z) {
			for (uint32_t y = 0; y < this->chunkDataSize.y; ++y) {
				const int32_t chunkX = this->chunkDataOffset.x + x;
				const int32_t chunkY = this->chunkDataOffset.y + y;
				const int32_t chunkZ = this->chunkDataOffset.z + z;
				const size_t oldIndex = ::getChunkIndex(chunkX,
				                                        chunkY,
				                                        chunkZ,
				                                        this->chunkDataOffset,
				                                        this->chunkDataSize);
				const size_t newIndex = ::getChunkIndex(chunkX,
				                                        chunkY,
				                                        chunkZ,
				                                        newChunkDataOffset,
				                                        newChunkDataSize);
				newChunkData[newIndex] = std::move(this->chunkData[oldIndex]);
			}
		}
	}

	this->chunkData = std::move(newChunkData);
	this->chunkDataOffset = newChunkDataOffset;
	this->chunkDataSize = newChunkDataSize;
}

void ChunkDataSystem::shrinkChunkData()
{
	// Calculate the new offset and size
	glm::ivec3 newChunkDataOffset;
	glm::uvec3 newChunkDataSize;
	bool initialized = false;

	for (uint32_t x = 0; x < this->chunkDataSize.x; ++x) {
		for (uint32_t z = 0; z < this->chunkDataSize.z; ++z) {
			for (uint32_t y = 0; y < this->chunkDataSize.y; ++y) {
				const int32_t chunkX = this->chunkDataOffset.x + x;
				const int32_t chunkY = this->chunkDataOffset.y + y;
				const int32_t chunkZ = this->chunkDataOffset.z + z;
				const size_t chunkIndex = this->getChunkIndex(chunkX,
				                                              chunkY,
				                                              chunkZ);
				if (this->chunkData[chunkIndex]) {
					if (!initialized) {
						initialized = true;
						newChunkDataOffset.x = chunkX;
						newChunkDataOffset.y = chunkY;
						newChunkDataOffset.z = chunkZ;
						newChunkDataSize.x = 1;
						newChunkDataSize.y = 1;
						newChunkDataSize.z = 1;
					} else {
						growChunkDataBounds(chunkX,
						                    chunkY,
						                    chunkZ,
						                    newChunkDataOffset,
						                    newChunkDataSize);
					}
				}
			}
		}
	}

	// If the bounds didn't change, we're good
	if (newChunkDataOffset == this->chunkDataOffset &&
	    newChunkDataSize == this->chunkDataSize) {
		return;
	}

	// Resize our chunk data array
	std::vector<std::unique_ptr<ChunkData>> newChunkData;
	newChunkData.resize(newChunkDataSize.x *
	                    newChunkDataSize.y *
	                    newChunkDataSize.z);

	for (uint32_t x = 0; x < newChunkDataSize.x; ++x) {
		for (uint32_t z = 0; z < newChunkDataSize.z; ++z) {
			for (uint32_t y = 0; y < newChunkDataSize.y; ++y) {
				const int32_t chunkX = newChunkDataOffset.x + x;
				const int32_t chunkY = newChunkDataOffset.y + y;
				const int32_t chunkZ = newChunkDataOffset.z + z;
				const size_t oldIndex = ::getChunkIndex(chunkX,
				                                        chunkY,
				                                        chunkZ,
				                                        this->chunkDataOffset,
				                                        this->chunkDataSize);
				const size_t newIndex = ::getChunkIndex(chunkX,
				                                        chunkY,
				                                        chunkZ,
				                                        newChunkDataOffset,
				                                        newChunkDataSize);
				newChunkData[newIndex] = std::move(this->chunkData[oldIndex]);
			}
		}
	}

	this->chunkData = std::move(newChunkData);
	this->chunkDataOffset = newChunkDataOffset;
	this->chunkDataSize = newChunkDataSize;
}
