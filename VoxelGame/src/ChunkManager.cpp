#include "ChunkManager.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VoxelChunk.h"
#include "Renderer.h"
#include <iostream>

ChunkManager::ChunkManager()
	:chunkCount(0)
{
	//Layout of buffer is position/normal/color
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(3);
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
	//find all chunks within render distance and check if generated
	bool currentgenerated = false;
	int chunksGenerated = 0;
	for (int i = currentChunk.x-HorizontalRenderDistance; i <= currentChunk.x + HorizontalRenderDistance; i++)
	{
		for (int j = currentChunk.y - verticalRenderDistance; j <= currentChunk.y + verticalRenderDistance; j++)
		{
			for (int k = currentChunk.z - HorizontalRenderDistance; k <= currentChunk.z + HorizontalRenderDistance; k++)
			{
				for (int l = 0; l < ActiveChunks.size(); l++)
				{
					if (ActiveChunks[l]->getChunkLocation() == glm::i32vec3(i,j,k))
					{
						currentgenerated = true;
						break;
					}
				}
				if (currentgenerated == false)
				{
					VoxelChunk* tempChunk;
					tempChunk = new VoxelChunk(glm::i32vec3(i,j,k));
					ActiveChunks.push_back(tempChunk);
					
					chunksGenerated++;
					if (chunksGenerated >= chunkUpdatesPerFrame)
						return;
				}
				currentgenerated = false;
			}
		}
	}
	
}

void ChunkManager::drawChunks(const Renderer& rend,Shader& shad,Camera& cam)
{
	glm::mat4 mvp;
	for (int i = 0; i < ActiveChunks.size(); i++)
	{
		if (ActiveChunks[i]->getIndexBuffer() == nullptr || ActiveChunks[i]->getVertexBuffer() == nullptr)

		{
			std::cerr << "Null pointer for index buffer/vertex buffer" << std::endl;
			exit(1);
		}
		else
		{

			//Create MVP matrix
			glm::mat4 mod = glm::translate(glm::mat4(1.0f), glm::vec3(ActiveChunks[i]->getChunkLocation()*chunksize));
			mvp = cam.GetProjMatrix() * cam.GetViewMatrix() * mod;
			shad.Bind();
			shad.SetUniformMat4f("u_MVP", mvp);
			shad.Unbind();

			va.AddBuffer(*ActiveChunks[i]->getVertexBuffer(), vbl);
			rend.Draw(va, *ActiveChunks[i]->getIndexBuffer(), shad);
		}
	}
	

}

std::vector<VoxelChunk*> ChunkManager::getActiveChunks()
{
	return ActiveChunks;
}
