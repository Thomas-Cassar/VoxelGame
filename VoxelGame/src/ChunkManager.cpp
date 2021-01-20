#include "ChunkManager.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VoxelChunk.h"
#include "Renderer.h"

ChunkManager::ChunkManager()
	:chunkCount(0)
{
}

ChunkManager::~ChunkManager()
{
	for (int i = 0; i < ActiveChunks.size(); i++)
	{
		delete ActiveChunks[i];
	}
}

void ChunkManager::updateChunks(glm::i32vec3 currentChunk)
{
	bool currentgenerated = false;
	for (int i = 0; i < ActiveChunks.size(); i++)
	{
		if (ActiveChunks[i]->getChunkLocation() == currentChunk)
		{
			currentgenerated = true;
		}
	}
	if (currentgenerated == false)
	{
		VoxelChunk* tempChunk;
		tempChunk = new VoxelChunk(currentChunk);
		ActiveChunks.push_back(tempChunk);
	}
	
}

std::vector<VoxelChunk*> ChunkManager::getActiveChunks()
{
	return ActiveChunks;
}
