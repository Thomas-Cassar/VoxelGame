#include "ChunkManager.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VoxelChunk.h"
#include "Renderer.h"
#include "VoxelConstants.h"
#include <iostream>

/*
*Reference: http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf
* Gill Gribb and Klaus Hartmann 
* Fast Frustum Plane Extraction
*/
bool ChunkManager::isinFrustrum(glm::mat4 perspective, glm::i32vec3 chunkLocation)
{
	glm::vec4 chunkCorner;
	glm::vec4 modifiedCorner;

	//Loop through all 8 corners of a chunk and see if any reside inside camera frustrum
	for (int i = 0; i < 8;i++)
	{
		switch (i)
		{//Cases for 8 differenc corners
		case 0:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize, (float)chunkLocation.y * chunksize, (float)chunkLocation.z * chunksize, 1.0f);
			break;
		case 1:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize+chunksize, (float)chunkLocation.y * chunksize, (float)chunkLocation.z * chunksize, 1.0f);
			break;
		case 2:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize, (float)chunkLocation.y * chunksize, (float)chunkLocation.z * chunksize + chunksize, 1.0f);
			break;
		case 3:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize + chunksize, (float)chunkLocation.y * chunksize, (float)chunkLocation.z * chunksize + chunksize, 1.0f);
			break;
		case 4:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize, (float)chunkLocation.y * chunksize + chunksize, (float)chunkLocation.z * chunksize, 1.0f);
			break;
		case 5:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize + chunksize, (float)chunkLocation.y * chunksize + chunksize, (float)chunkLocation.z * chunksize, 1.0f);
			break;
		case 6:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize, (float)chunkLocation.y * chunksize + chunksize, (float)chunkLocation.z * chunksize + chunksize, 1.0f);
			break;
		case 7:
			chunkCorner = glm::vec4((float)chunkLocation.x * chunksize + chunksize, (float)chunkLocation.y * chunksize + chunksize, (float)chunkLocation.z * chunksize + chunksize, 1.0f);
			break;
		}


		//If one corner is inside then the function returns true
				modifiedCorner = perspective * chunkCorner;
		if (modifiedCorner.x > -1 * modifiedCorner.w && modifiedCorner.x < modifiedCorner.w &&
			modifiedCorner.y>-1 * modifiedCorner.w && modifiedCorner.y < modifiedCorner.w &&
			modifiedCorner.z>-1 * modifiedCorner.w && modifiedCorner.z < modifiedCorner.w)
			return true;
	}
	
	//No corners inside
	return false;
}

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
		if (ActiveChunks[i]->getisEmpty())
		{
			//If chunk is empty do nothing
		}
		else if (ActiveChunks[i]->getIndexBuffer() == nullptr || ActiveChunks[i]->getVertexBuffer() == nullptr)

		{
			std::cerr << "Null pointer for index buffer/vertex buffer" << std::endl;
			exit(1);
		}
		else
		{
			if (isinFrustrum(cam.GetProjMatrix() *cam.GetViewMatrix(),ActiveChunks[i]->getChunkLocation()))
			{
				//Create MVP matrix
				glm::mat4 mod = glm::translate(glm::mat4(1.0f), glm::vec3(ActiveChunks[i]->getChunkLocation() * chunksize));
				mvp = cam.GetProjMatrix() * cam.GetViewMatrix() * mod;
				shad.Bind();
				shad.SetUniformMat4f("u_MVP", mvp);
				shad.Unbind();

				va.AddBuffer(*ActiveChunks[i]->getVertexBuffer(), vbl);
				rend.Draw(va, *ActiveChunks[i]->getIndexBuffer(), shad);
			}
		}
	}
	

}

std::vector<VoxelChunk*> ChunkManager::getActiveChunks()
{
	return ActiveChunks;
}
