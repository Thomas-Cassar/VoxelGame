#pragma once
#include "glm\glm.hpp"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VoxelConstants.h"

class VoxelChunk {
private:
	unsigned char VoxelData[chunksize+2][chunksize+2][chunksize+2];
	glm::i32vec3 chunkLocation;
	int vertexfloatcount, indexcount;
	int vertexmax=1500000, indexmax=1000000;//Initial size of array of vertex and index until reallocated into actual size
	VertexBuffer* vb;
	IndexBuffer* ib;
public:
	VoxelChunk(glm::i32vec3 world);
	~VoxelChunk();
	void calculateGeometry();
	int getVertexFloatCount();
	int getIndexCount();
	VertexBuffer* getVertexBuffer();
	IndexBuffer* getIndexBuffer();
	glm::i32vec3 getChunkLocation();
};