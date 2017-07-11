#include <GL/glew.h>

#include "chunk-mesh.hpp"

ChunkMesh::ChunkMesh()
	: vertexCount(0),
	  indexCount(0)
{
	glGenBuffers(1, &this->vertexVboId);
	glGenBuffers(1, &this->indexVboId);
}

ChunkMesh::~ChunkMesh()
{
	glDeleteBuffers(1, &this->vertexVboId);
	glDeleteBuffers(1, &this->indexVboId);
}
