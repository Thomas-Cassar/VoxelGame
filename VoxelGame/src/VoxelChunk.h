#pragma once
#include "glm\glm.hpp"

#define chunksize 32

class VoxelChunk {
private:
	unsigned char VoxelData[chunksize+2][chunksize+2][chunksize+2];
	glm::i32vec3 chunkLocation;
	int vertexfloatcount, indexcount;
	int vertexmax=1500000, indexmax=1000000;//Initial size of array of vertex and index until reallocated into actual size
	float* vertexarray;
	unsigned int* indexarray;
public:
	VoxelChunk(glm::i32vec3 world);
	~VoxelChunk();
	void calculateGeometry();
	float* getVertexArray();
	int getVertexFloatCount();
	unsigned int* getIndexArray();
	int getIndexCount();
	glm::i32vec3 getChunkLocation();
};