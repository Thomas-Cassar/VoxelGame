#include "VoxelChunk.h"
#include "SimplexNoise\SimplexNoise.h"
#include "Transvoxel.h"

#include <iostream>

#define vertexatributecount 9

glm::vec3 calculateNormal(int vertexArrayIndex, float* vertexarray)
{
	//Get location of 3 verticies
	glm::vec3 Index0(vertexarray[vertexArrayIndex - vertexatributecount * 3], vertexarray[vertexArrayIndex - vertexatributecount * 3 + 1], vertexarray[vertexArrayIndex - vertexatributecount * 3 + 2]);
	glm::vec3 Index1(vertexarray[vertexArrayIndex - vertexatributecount * 2], vertexarray[vertexArrayIndex - vertexatributecount * 2 + 1], vertexarray[vertexArrayIndex - vertexatributecount * 2 + 2]);
	glm::vec3 Index2(vertexarray[vertexArrayIndex - vertexatributecount * 1], vertexarray[vertexArrayIndex - vertexatributecount * 1 + 1], vertexarray[vertexArrayIndex - vertexatributecount * 1 + 2]);

	//Get location realtive to index0
	Index0 = Index0 - Index1;
	Index2 = Index2 - Index1;

	//Return cross product
	return glm::normalize(glm::cross(Index0, Index2));
}

VoxelChunk::VoxelChunk(glm::i32vec3 world)
	:chunkLocation(world),
	vertexfloatcount(0),
	indexcount(0)
{
	SimplexNoise cnoise;
	int i, j, k;
	for (i = 0; i < chunksize+1; i++)
	{
		for (j = 0; j < chunksize + 1; j++)
		{
			for (k = 0; k < chunksize + 1; k++)
			{
				if (k<j)
				VoxelData[i][j][k] = 1;
				else
				VoxelData[i][j][k] = -1;
			}
		}
	}

	for (i = 0; i < chunksize; i++)
	{
		for (j = 0; j < chunksize; j++)
		{
			for (k = 0; k < chunksize; k++)
			{
				unsigned char caseCode = ((VoxelData[i][j][k] >> 7) & 0x01)
					| ((VoxelData[i + 1][j][k] >> 6) & 0x02)
					| ((VoxelData[i][j + 1][k] >> 5) & 0x04)
					| ((VoxelData[i + 1][j + 1][k] >> 4) & 0x08)
					| ((VoxelData[i][j][k + 1] >> 3) & 0x10)
					| ((VoxelData[i + 1][j][k + 1] >> 2) & 0x20)
					| ((VoxelData[i][j + 1][k + 1] >> 1) & 0x40)
					| (VoxelData[i + 1][j + 1][k + 1] & 0x80);

				vertexfloatcount+= regularCellData[regularCellClass[caseCode]].GetVertexCount();
				indexcount += 3 * regularCellData[regularCellClass[caseCode]].GetTriangleCount();
			}
		}
	}
	vertexfloatcount *= vertexatributecount;

	indexarray = new unsigned int[indexcount];
	
	vertexarray = new float[vertexfloatcount];

	int triIndex = 0;

	int ind = 0, ver = 0,l;
	for (i = 0; i < chunksize; i++)
	{
		for (j = 0; j < chunksize; j++)
		{
			for (k = 0; k < chunksize; k++)
			{
				unsigned char caseCode = ((VoxelData[i][j][k] >> 7) & 0x01)
					| ((VoxelData[i + 1][j][k] >> 6) & 0x02)
					| ((VoxelData[i][j + 1][k] >> 5) & 0x04)
					| ((VoxelData[i + 1][j + 1][k] >> 4) & 0x08)
					| ((VoxelData[i][j][k + 1] >> 3) & 0x10)
					| ((VoxelData[i + 1][j][k + 1] >> 2) & 0x20)
					| ((VoxelData[i][j + 1][k + 1] >> 1) & 0x40)
					| (VoxelData[i + 1][j + 1][k + 1] & 0x80);
				

				for (l = 0; l < 3 * regularCellData[regularCellClass[caseCode]].GetTriangleCount(); l++)
				{
					indexarray[ind] = ver / vertexatributecount + regularCellData[regularCellClass[caseCode]].vertexIndex[l];
					ind++;
				}

				for (l = 0; l < regularCellData[regularCellClass[caseCode]].GetVertexCount(); l++)
				{
					switch (regularVertexData[caseCode][l]&0xFF)
					{
					case 0x01:
						vertexarray[ver] = (float)i + 0.5f;
						ver++;
						vertexarray[ver] = (float)j;
						ver++;
						vertexarray[ver] = (float)k;
						ver++;
						break;
					case 0x13:
						vertexarray[ver] = (float)i + 1.0f;
						ver++;
						vertexarray[ver] = (float)j + 0.5f;
						ver++;
						vertexarray[ver] = (float)k;
						ver++;
						break;
					case 0x23:
						vertexarray[ver] = (float)i + 0.5f;
						ver++;
						vertexarray[ver] = (float)j + 1.0f;
						ver++;
						vertexarray[ver] = (float)k ;
						ver++;
						break;
					case 0x02:
						vertexarray[ver] = (float)i;
						ver++;
						vertexarray[ver] = (float)j+0.5f ;
						ver++;
						vertexarray[ver] = (float)k;
						ver++;
						break;
					case 0x04:
						vertexarray[ver] = (float)i;
						ver++;
						vertexarray[ver] = (float)j ;
						ver++;
						vertexarray[ver] = (float)k + 0.5f;
						ver++;
						break;
					case 0x15:
						vertexarray[ver] = (float)i+1.0f;
						ver++;
						vertexarray[ver] = (float)j;
						ver++;
						vertexarray[ver] = (float)k + 0.5f;
						ver++;
						break;
					case 0x37:
						vertexarray[ver] = (float)i + 1.0f;
						ver++;
						vertexarray[ver] = (float)j + 1.0f;
						ver++;
						vertexarray[ver] = (float)k + 0.5f;
						ver++;
						break;
					case 0x26:
						vertexarray[ver] = (float)i;
						ver++;
						vertexarray[ver] = (float)j + 1.0f;
						ver++;
						vertexarray[ver] = (float)k + 0.5f;
						ver++;
						break;
					case 0x45:
						vertexarray[ver] = (float)i + 0.5f;
						ver++;
						vertexarray[ver] = (float)j;
						ver++;
						vertexarray[ver] = (float)k + 1.0f;
						ver++;
						break;
					case 0x57:
						vertexarray[ver] = (float)i + 1.0f;
						ver++;
						vertexarray[ver] = (float)j+0.5f;
						ver++;
						vertexarray[ver] = (float)k + 1.0f;
						ver++;
						break;
					case 0x67:
						vertexarray[ver] = (float)i + 0.5f;
						ver++;
						vertexarray[ver] = (float)j+1.0f;
						ver++;
						vertexarray[ver] = (float)k + 1.0f;
						ver++;
						break;
					case 0x46: 
						vertexarray[ver] = (float)i;
						ver++;
						vertexarray[ver] = (float)j+0.5f;
						ver++;
						vertexarray[ver] = (float)k + 1.0f;
						ver++;
						break;
					}
					// Add default normal value
					if (triIndex <3)
					{
						vertexarray[ver] = 1.0f;
						ver++;
						vertexarray[ver] = 1.0f;
						ver++;
						vertexarray[ver] = 1.0f;
						ver++;
						triIndex++;
					}
					//Add color
						vertexarray[ver] = 0.1f;
						ver++;
						vertexarray[ver] = 0.8f;
						ver++;
						vertexarray[ver] = 0.1f;
						ver++;
					//Every 3 indicies calulate normal
						if (triIndex >= 3)
						{
							glm::vec3 normal = calculateNormal(ver, vertexarray);

							//Change already set normal
							vertexarray[(ver - vertexatributecount * 3) + 3] = normal.x;
							vertexarray[(ver - vertexatributecount * 2) + 3] = normal.x;
							vertexarray[(ver - vertexatributecount * 1) + 3] = normal.x;

							vertexarray[(ver - vertexatributecount * 3) + 4] = normal.y;
							vertexarray[(ver - vertexatributecount * 2) + 4] = normal.y;
							vertexarray[(ver - vertexatributecount * 1) + 4] = normal.y;

							vertexarray[(ver - vertexatributecount * 3) + 5] = normal.z;
							vertexarray[(ver - vertexatributecount * 2) + 5] = normal.z;
							vertexarray[(ver - vertexatributecount * 1) + 5] = normal.z;

							triIndex = 0;
						}
					
				}
			}
		}
	}
	std::cout << ver/vertexatributecount << "/" << vertexfloatcount/ vertexatributecount << "verticies made/vertcices allocated " << ind << "/" << indexcount << " indicies made/indicies allocated" << std::endl;
}



VoxelChunk::~VoxelChunk()
{
	delete vertexarray;
	delete indexarray;
}

float* VoxelChunk::getVertexArray()
{
	return vertexarray;
}

int VoxelChunk::getVertexFloatCount()
{
	return vertexfloatcount;
}

unsigned int* VoxelChunk::getIndexArray()
{
	return indexarray;
}

int VoxelChunk::getIndexCount()
{
	return indexcount;
}

glm::i32vec3 VoxelChunk::getChunkLocation()
{
	return chunkLocation;
}



