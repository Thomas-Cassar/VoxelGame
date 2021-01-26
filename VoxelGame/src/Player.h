#pragma once
#include "glm\glm.hpp"
#include "Camera.h"
#include "GLFW\glfw3.h"

#include "VoxelConstants.h"

class Player {
private:
	Camera playerCamera;
	GLFWwindow* windowPointer;
	float playerSpeed = 25.0f;
public:
	Player(int windowwidth,int windowheight,GLFWwindow* windowContext);
	glm::vec3 getworldLocation();
	glm::i32vec3 getChunkLocation();
	Camera* getPlayerCamera();
	void updatePlayerInput(float delta);
};