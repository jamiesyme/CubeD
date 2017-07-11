#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "block.hpp"
#include "chunk-data.hpp"
#include "chunk-data-system.hpp"
#include "chunk-mesh.hpp"
#include "chunk-mesh-system.hpp"
#include "world.hpp"

namespace {
	struct Vertex {
		float x;
		float y;
		float z;
	};

	struct Triangle {
		uint32_t i[3];
	};
}

ChunkMeshSystem::ChunkMeshSystem(World& world)
	: world(world)
{
}

void ChunkMeshSystem::render() const
{
	// Pre-render
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);

	// Render each chunk mesh
	for (const auto& chunkMesh : this->chunkMeshes) {
		glBindBuffer(GL_ARRAY_BUFFER, chunkMesh->vertexVboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh->indexVboId);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawElements(GL_TRIANGLES, chunkMesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	// Post-render
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkMeshSystem::mapChunkMesh(const int32_t chunkX,
                                   const int32_t chunkY,
                                   const int32_t chunkZ)
{
	// Check for an existing chunk mesh
	for (const auto& chunkMesh : this->chunkMeshes) {
		if (chunkMesh->chunkX == chunkX &&
		    chunkMesh->chunkY == chunkY &&
		    chunkMesh->chunkZ == chunkZ) {
			return;
		}
	}

	// Create the chunk mesh as it doesn't exist
	this->chunkMeshes.emplace_back(new ChunkMeshWithIndex(chunkX,
	                                                      chunkY,
	                                                      chunkZ));
}

void ChunkMeshSystem::unmapChunkMesh(const int32_t chunkX,
                                     const int32_t chunkY,
                                     const int32_t chunkZ)
{
	// Find the chunk mesh and remove it
	for (auto it = this->chunkMeshes.begin();
	     it != this->chunkMeshes.end();
	     ++it) {
		if ((*it)->chunkX == chunkX &&
		    (*it)->chunkY == chunkY &&
		    (*it)->chunkZ == chunkZ) {
			this->chunkMeshes.erase(it);
			return;
		}
	}
}

void ChunkMeshSystem::updateChunkMesh(const int32_t chunkX,
                                      const int32_t chunkY,
                                      const int32_t chunkZ,
                                      const ChunkData& chunkData)
{
	// Find the chunk mesh and update it
	for (auto& chunkMesh : this->chunkMeshes) {
		if (chunkMesh->chunkX == chunkX &&
		    chunkMesh->chunkY == chunkY &&
		    chunkMesh->chunkZ == chunkZ) {
			updateChunkMesh(chunkData, *chunkMesh);
			return;
		}
	}
}

ChunkMeshSystem::ChunkMeshWithIndex::ChunkMeshWithIndex(const int32_t chunkX,
                                                        const int32_t chunkY,
                                                        const int32_t chunkZ)
	: chunkX(chunkX),
	  chunkY(chunkY),
	  chunkZ(chunkZ)
{
}

void ChunkMeshSystem::updateChunkMesh(const ChunkData& chunkData,
                                      ChunkMeshWithIndex& chunkMesh)
{
	const glm::uvec3 chunkSize = ChunkData::getSize();
	const int64_t blockOffsetX = chunkMesh.chunkX * chunkSize.x;
	const int64_t blockOffsetY = chunkMesh.chunkY * chunkSize.y;
	const int64_t blockOffsetZ = chunkMesh.chunkZ * chunkSize.z;
	const float blockSize = Block::getSize();
	const ChunkDataSystem& chunkDataSystem = this->world.chunkDataSystem;

	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	for (uint32_t x = 0; x < chunkSize.x; ++x) {
		for (uint32_t z = 0; z < chunkSize.z; ++z) {
			for (uint32_t y = 0; y < chunkSize.y; ++y) {
				const int64_t blockX = blockOffsetX + x;
				const int64_t blockY = blockOffsetY + y;
				const int64_t blockZ = blockOffsetZ + z;

				const BlockId centerBlockId = chunkDataSystem.getBlockId(blockX,
				                                                         blockY,
				                                                         blockZ);
				if (centerBlockId == BlockId::Void ||
				    centerBlockId == BlockId::Empty) {
					continue;
				}

				const float x1 = (float)x + blockSize / 4;
				const float x2 = (float)x + blockSize * 3 / 4;
				const float y1 = (float)y + blockSize / 4;
				const float y2 = (float)y + blockSize * 3 / 4;
				const float z1 = (float)z + blockSize / 2;
				const uint32_t i = vertices.size();
				vertices.push_back({x1, y1, z1});
				vertices.push_back({x2, y1, z1});
				vertices.push_back({x1, y2, z1});
				vertices.push_back({x2, y2, z1});
				triangles.push_back({i + 0, i + 1, i + 2});
				triangles.push_back({i + 3, i + 2, i + 1});
			}
		}
	}

	chunkMesh.vertexCount = vertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.vertexVboId);
	glBufferData(GL_ARRAY_BUFFER,
	             vertices.size() * sizeof(Vertex),
	             &vertices[0],
	             GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	chunkMesh.indexCount = triangles.size() * 3;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             triangles.size() * sizeof(Triangle),
	             &triangles[0],
	             GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
