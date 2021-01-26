#include "VoxelChunk.h"
#include "SimplexNoise\SimplexNoise.h"
#include "Transvoxel.h"

#include <iostream>

VoxelChunk::VoxelChunk(glm::i32vec3 world)
	:chunkLocation(world),
	vertexfloatcount(0),
	indexcount(0),
	ib(nullptr),
	vb(nullptr)
{
	SimplexNoise cnoise;
	int i, j, k;
	for (i = 0; i < chunksize+2; i++)
	{
		for (j = 0; j < chunksize + 2; j++)
		{
			for (k = 0; k < chunksize + 2; k++)
			{
				if (((cnoise.noise((float)(i+chunkLocation.x*chunksize-1) / 30, (float)(k+chunkLocation.z*chunksize-1) / 30) + 1) * 10) > (j+chunkLocation.y * chunksize - 1))
				//cube test
				//if(i==0||j==0||k==0||i==chunksize+1||j==chunksize+1||k==chunksize+1)
				VoxelData[i][j][k] = 1;
				else
				VoxelData[i][j][k] = 0;
			}
		}
	}
	calculateGeometry();
	
}



VoxelChunk::~VoxelChunk()
{
	delete ib;
	delete vb;
}

void VoxelChunk::calculateGeometry()
{
	//Create two arrays at their maximum size (reduced after number is known)
	float* vertexarray = new float[vertexmax];
	unsigned int* indexarray = new unsigned int[indexmax];
	//Counter variables for each array
	vertexfloatcount = 0;
	indexcount = 0;

	//3D loop variables
	int i, j, k;

	//Loop through every voxel
	for (i = 1; i < chunksize + 1; i++)
	{
		for (j = 1; j < chunksize + 1; j++)
		{
			for (k = 1; k < chunksize + 1; k++)
			{
				//If there's a solid block we check each face to see if it borders air
				if (VoxelData[i][j][k] != 0)
				{
					//Check +x
					if (VoxelData[i + 1][j][k] == 0)//If empty in positve X direction add face at +x
					{
						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
					}

					//Check -X
					if (VoxelData[i - 1][j][k] == 0)//If empty in negative X direction add face at -x
					{
						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
					}

					//Check +y
					if (VoxelData[i][j + 1][k] == 0)//If empty in positve Y direction add face at +Y
					{
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal and Color
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;

						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal and Color
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;

						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal and Color
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;

						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal and Color
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
					}

					//Check -y
					if (VoxelData[i][j - 1][k] == 0)//If empty in negative Y direction add face at -Y
					{
						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
					}

					//Check +Z
					if (VoxelData[i][j][k + 1] == 0)//If empty in positve Z direction add face at +Z
					{
						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;
						
						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k + 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
					}

					//Check -Z
					if (VoxelData[i][j][k - 1] == 0)//If empty in negative Z direction add face at -Z
					{
						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = -1.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Location
						vertexarray[vertexfloatcount] = (float)i - 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)j + 0.5f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = (float)k - 0.5f;
						vertexfloatcount++;
						//Normal
						vertexarray[vertexfloatcount] = 1.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.0f;
						vertexfloatcount++;
						//Color
						vertexarray[vertexfloatcount] = 0.39f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.26f;
						vertexfloatcount++;
						vertexarray[vertexfloatcount] = 0.13f;
						vertexfloatcount++;

						//Triangles
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 3;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;

						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 4;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 2;
						indexcount++;
						indexarray[indexcount] = vertexfloatcount / vertexatributecount - 1;
						indexcount++;
					}
				}
			}
		}
	}

	//Create vertex buffer out of calculated data
	vb = new VertexBuffer(vertexarray,vertexfloatcount*sizeof(float));
	//Create index buffer out of calculated data
	ib = new IndexBuffer(indexarray, indexcount);

	//Delete temp buffers
	delete[] vertexarray;
	delete[] indexarray;
}


int VoxelChunk::getVertexFloatCount()
{
	return vertexfloatcount;
}

int VoxelChunk::getIndexCount()
{
	return indexcount;
}

VertexBuffer* VoxelChunk::getVertexBuffer()
{
	return vb;
}

IndexBuffer* VoxelChunk::getIndexBuffer()
{
	return ib;
}

glm::i32vec3 VoxelChunk::getChunkLocation()
{
	return chunkLocation;
}



