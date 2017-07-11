#pragma once

#include <cstdint>

class ChunkMesh {
public:
	ChunkMesh();

	~ChunkMesh();

	unsigned int vertexVboId;
	unsigned int indexVboId;
	unsigned int vertexCount;
	unsigned int indexCount;
};
