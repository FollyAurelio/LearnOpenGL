#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 startPosition, glm::vec3 startDirection)
{
	position = startPosition;
	direction = startDirection;
	sensitivity = 0.1f;
	yaw = -90.0f;
	pitch = 0.0f;
	speed = 0.05f;
	mouseXLast = 400.0f;
	mouseXLast = 300.0f;
	firstMouseMovement = true;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
}
void Camera::setDirection(float mouseXPosition, float mouseYPosition)
{
	if(firstMouseMovement){
		mouseXLast = mouseXPosition;
		mouseYLast = mouseYPosition;
		firstMouseMovement = false;
	}
	float mouseXOffset = (mouseXPosition - mouseXLast) * sensitivity;
	float mouseYOffset = (mouseYLast - mouseYPosition) * sensitivity;
	mouseXLast = mouseXPosition;
	mouseYLast = mouseYPosition;
	yaw += mouseXOffset;
	pitch += mouseYOffset;
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);
}

void Camera::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void Camera::movePosition(CameraMovementType cameraMovementType)
{
	switch(cameraMovementType)
	{
		case CAMERA_MOVE_FOREWARD:
			position += speed * direction;
			break;
		case CAMERA_MOVE_BACKWARD:
			position -= speed * direction;
			break;
		case CAMERA_MOVE_LEFT:
			position -= glm::normalize(glm::cross(direction, worldUp)) * speed;
			break;
		case CAMERA_MOVE_RIGHT:
			position += glm::normalize(glm::cross(direction, worldUp)) * speed;
			break;
		case CAMERA_MOVE_UP:
			position += worldUp * speed;
			break;
		case CAMERA_MOVE_DOWN:
			position -= worldUp * speed;
			break;
	}
		

}

void Camera::setViewMatrix()
{
	view = glm::lookAt(position, position + direction, worldUp);
}

glm::mat4 Camera::getViewMatrix()
{
	return view;
}


	

	

