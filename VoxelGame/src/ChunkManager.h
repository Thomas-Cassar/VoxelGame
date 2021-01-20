#pragma once
#include "VoxelChunk.h"
#include <vector>
#include "glm\glm.hpp"

class ChunkManager {
private:
	int chunkCount;
	std::vector<VoxelChunk*> ActiveChunks;
public:
	ChunkManager();
	~ChunkManager();
	void updateChunks(glm::i32vec3 currentChunk);
	std::vector <VoxelChunk*> getActiveChunks();
};