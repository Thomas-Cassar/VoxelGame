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
	glm::i32vec3 prevLocation= glm::i32vec3(0, 0, 0);
	VoxelChunk* ActiveChunks[HorizontalRenderDistance * 2 + 1][verticalRenderDistance * 2 + 1][HorizontalRenderDistance * 2 + 1] = { nullptr };
	VertexArray va;
	VertexBufferLayout vbl;
	bool isinFrustrum(glm::mat4 perspective,glm::i32vec3 chunkLocation);
	int modPos(int a, int b);
public:
	ChunkManager();
	~ChunkManager();
	void updateChunks(glm::i32vec3 currentChunk);
	void drawChunks(const Renderer& rend, Shader& shad,Camera& cam);
};