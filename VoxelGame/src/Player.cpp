#include "Player.h"

Player::Player(int windowwidth, int windowheight, GLFWwindow* windowContext)
	:playerCamera(windowwidth,windowheight),
	windowPointer(windowContext)
{
}

glm::vec3 Player::getworldLocation()
{
	return playerCamera.GetTranslation();
}

glm::i32vec3 Player::getChunkLocation()
{
	return glm::i32vec3(
		static_cast <int> (std::floor(getworldLocation().x / chunksize)), 
		static_cast <int> (std::floor(getworldLocation().y / chunksize)), 
		static_cast <int> (std::floor(getworldLocation().z / chunksize)));
}

Camera* Player::getPlayerCamera()
{
	return &playerCamera;
}

void Player::updatePlayerInput(float delta)
{
	if (glfwGetKey(windowPointer, GLFW_KEY_W) == GLFW_PRESS)
	{
		playerCamera.moveForward(delta*playerSpeed);
	}

	if (glfwGetKey(windowPointer, GLFW_KEY_S) == GLFW_PRESS)
	{
		playerCamera.moveForward(delta * playerSpeed*-1.0f);
	}
	if (glfwGetKey(windowPointer, GLFW_KEY_A) == GLFW_PRESS)
	{
		playerCamera.moveRight(delta * playerSpeed * -1.0f);
	}
	if (glfwGetKey(windowPointer, GLFW_KEY_D) == GLFW_PRESS)
	{
		playerCamera.moveRight(delta * playerSpeed );
	}
}


