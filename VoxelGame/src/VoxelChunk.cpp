#include "VoxelChunk.h"
#include "SimplexNoise\SimplexNoise.h"
#include "Transvoxel.h"

#include <iostream>

#define vertexatributecount 6

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
				if (((cnoise.noise((float)(i) / 30, (float)(k) / 30) + 1) * 10) < j)
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

	int color = 0;

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
					if (color == 0)
					{
						vertexarray[ver] = 0.1f;
						ver++;
						vertexarray[ver] = 0.8f;
						ver++;
						vertexarray[ver] = 0.1f;
						ver++;
						color++;
					}
					else if (color == 1)
					{
						vertexarray[ver] = 0.1f;
						ver++;
						vertexarray[ver] = 0.8f;
						ver++;
						vertexarray[ver] = 0.1f;
						ver++;
						color++;
					}
					else
					{
						vertexarray[ver] = 0.1f;
						ver++;
						vertexarray[ver] = 0.8f;
						ver++;
						vertexarray[ver] = 0.1f;
						ver++;
						color++;
						color = 0;
					}
				}
			}
		}
	}
	std::cout << ver << "/" << vertexfloatcount << "verticies made/vertcices allocated " << ind << "/" << indexcount << " indicies made/indicies allocated" << std::endl;
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

