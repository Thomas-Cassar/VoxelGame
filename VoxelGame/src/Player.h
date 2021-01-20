#pragma once
#include "glm\glm.hpp"
#include "Camera.h"
#include "GLFW\glfw3.h"

class Player {
private:
	Camera playerCamera;
	GLFWwindow* windowPointer;
	float playerSpeed = 50.0f;
public:
	Player(int windowwidth,int windowheight,GLFWwindow* windowContext);
	glm::vec3 getworldLocation();
	Camera* getPlayerCamera();
	void updatePlayerInput(float delta);
};