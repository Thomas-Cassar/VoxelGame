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

int ChunkManager::modPos(int a, int b)
{
	int c = a % b;
	return c>=0 ?c:c+b;
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
	for (int i = 0; i < HorizontalRenderDistance * 2 + 1; i++)
	{
		for (int j = 0; j < verticalRenderDistance * 2 + 1; j++)
		{
			for (int k = 0; k < HorizontalRenderDistance * 2 + 1; k++)
			{
				delete ActiveChunks[i][j][k];
			}
		}
	}
		
}

void ChunkManager::updateChunks(glm::i32vec3 currentChunk)
{
	//If all in render distance generated and in same chunk then no update needed
	if ((chunkCount == maxChunks)&&prevLocation==currentChunk)
		return;
	prevLocation = currentChunk;//Set previous location for next check

	//find all chunks within render distance and check if generated
	int chunksGenerated = 0;
	
	for (int i = currentChunk.x-HorizontalRenderDistance; i <= currentChunk.x+HorizontalRenderDistance; i++)
	{
		for (int j = currentChunk.y-verticalRenderDistance; j <= currentChunk.y+verticalRenderDistance; j++)
		{
			for (int k = currentChunk.z - HorizontalRenderDistance; k <= currentChunk.z + HorizontalRenderDistance; k++)
			{
				//Array index values for chunk were looking at 
				int x = modPos(i, HorizontalRenderDistance * 2 + 1);
				int y = modPos(j, verticalRenderDistance * 2 + 1);
				int z = modPos(k, HorizontalRenderDistance * 2 + 1);

				//We see if this location of the chunk is out of render distance and if so delete it
				if (ActiveChunks[x][y][z] != nullptr)
				{
					glm::i32vec3 loc = ActiveChunks[x][y][z]->getChunkLocation();
					if (loc.x< currentChunk.x - HorizontalRenderDistance || loc.x>currentChunk.x + HorizontalRenderDistance ||
						loc.z< currentChunk.z - HorizontalRenderDistance || loc.z>currentChunk.z + HorizontalRenderDistance ||
						loc.y< currentChunk.y - verticalRenderDistance || loc.y>currentChunk.y + verticalRenderDistance
						)
					{
						delete ActiveChunks[x][y][z];
						ActiveChunks[x][y][z] = nullptr;
						chunkCount--;
					}
				}

				//If this chunk is not generated and we have not exceeded max updates this frame we make new chunk
				if (ActiveChunks[x][y][z] == nullptr && chunksGenerated < chunkUpdatesPerFrame)
				{
					//If not created create a new chunk at specified index and position in render distance
					ActiveChunks[x][y][z] = new VoxelChunk(glm::i32vec3(i, j, k));

					//Add to amount of chunks generated in this update
					chunksGenerated++;	

					//Add to total amount of chunks
					chunkCount++;
				}
			}
		}
	}
	
}


void ChunkManager::drawChunks(const Renderer& rend,Shader& shad,Camera& cam)
{
	glm::mat4 mvp;
	for (int i = 0; i < HorizontalRenderDistance*2+1; i++)
	{
		for (int j = 0; j < verticalRenderDistance * 2 + 1; j++)
		{
			for (int k = 0; k < HorizontalRenderDistance * 2 + 1; k++)
			{
				if (ActiveChunks[i][j][k] == nullptr)
				{
					//If not generated do nothing
				}
				else if (ActiveChunks[i][j][k]->getisEmpty())
				{
					//If chunk is empty do nothing
				}
				else if (ActiveChunks[i][j][k]->getIndexBuffer() == nullptr || ActiveChunks[i][j][k]->getVertexBuffer() == nullptr)

				{
					std::cerr << "Null pointer for index buffer/vertex buffer" << std::endl;
					exit(1);
				}
				else
				{
					if (isinFrustrum(cam.GetProjMatrix() * cam.GetViewMatrix(), ActiveChunks[i][j][k]->getChunkLocation()))
					{
						//Create MVP matrix
						glm::mat4 mod = glm::translate(glm::mat4(1.0f), glm::vec3(ActiveChunks[i][j][k]->getChunkLocation() * chunksize));
						mvp = cam.GetProjMatrix() * cam.GetViewMatrix() * mod;
						shad.Bind();
						shad.SetUniformMat4f("u_MVP", mvp);
						shad.Unbind();

						va.AddBuffer(*ActiveChunks[i][j][k]->getVertexBuffer(), vbl);
						rend.Draw(va, *ActiveChunks[i][j][k]->getIndexBuffer(), shad);
					}
				}
			}
			}
		}
		
}
