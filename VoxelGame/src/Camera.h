#pragma once
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
	glm::vec3 Translation;
	glm::vec3 Direction;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);
	float lastmousex, lastmousey;
	bool firstmouse;
	float yaw, pitch;
	float sensitivity=.1f;
	float cameraspeed = .1f;

public:
	Camera();
	~Camera();
	glm::vec3 GetTranslation();
	glm::mat4 GetViewMatrix();
	void moveForward();
	void moveBackward();
	void moveRight();
	void moveLeft();
	void newMousepos(float x,float y);
};
	
