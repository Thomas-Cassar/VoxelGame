#pragma once
#include "VoxelChunk.h"
#include <vector>
#include "glm\glm.hpp"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Camera.h"




class ChunkManager {
private:
	int chunkCount;
	std::vector<VoxelChunk*> ActiveChunks;
	VertexArray va;
	VertexBufferLayout vbl;
public:
	ChunkManager();
	~ChunkManager();
	void updateChunks(glm::i32vec3 currentChunk);
	void drawChunks(const Renderer& rend, Shader& shad,Camera& cam);
	std::vector <VoxelChunk*> getActiveChunks();
};