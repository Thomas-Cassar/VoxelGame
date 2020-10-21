#pragma once
#include "glm\glm.hpp"

#define chunksize 50

class VoxelChunk {
private:
	signed char VoxelData[chunksize+1][chunksize+1][chunksize+1];
	glm::vec3 WorldLocation;
	int vertexfloatcount, indexcount;
	float* vertexarray;
	unsigned int* indexarray;
public:
	VoxelChunk(glm::vec3 world);
	~VoxelChunk();
	float* getVertexArray();
	int getVertexFloatCount();
	unsigned int* getIndexArray();
	int getIndexCount();

};