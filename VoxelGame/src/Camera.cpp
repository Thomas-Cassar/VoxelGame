#include "Camera.h"

Camera::Camera(int widthin, int heightin)
	:Translation(4.0f, 3.0f, 3.0f),
	lastmousex(0.0f),
	lastmousey(0.0f),
	pitch(0.0f),
	yaw(0.0f),
	firstmouse(false),
	Direction(glm::vec3(1, 0, 0)),
	cameraFront(glm::normalize(Direction) + Translation),
	width(widthin),
	height(heightin)
{
proj = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 1000.0f);
}

Camera::~Camera()
{

}

glm::vec3 Camera::GetTranslation()
{
	return Translation;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 viewmatrix = glm::lookAt(
		Translation,
		cameraFront+Translation,
		cameraUp
	);

	return viewmatrix;
}

glm::mat4 Camera::GetProjMatrix()
{
	return proj;
}

void Camera::moveForward(float distance)
{
	Translation +=  distance * cameraFront;
}


void Camera::moveRight(float distance)
{
	Translation += glm::normalize(glm::cross(cameraFront, cameraUp)) *  distance;
}


void Camera::newMousepos(float x, float y)
{
	//https://learnopengl.com/Getting-started/Camera

	if (firstmouse == true)
	{
		firstmouse = false;
		lastmousex = x;
		lastmousey = y;
		return;
	}

	float xoffset = x - lastmousex;
	float yoffset = lastmousey - y;

	lastmousex = x;
	lastmousey = y;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction.y = sin(glm::radians(pitch));
	Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(Direction);
}
