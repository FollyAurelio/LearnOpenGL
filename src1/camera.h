#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovementType
{
	CAMERA_MOVE_FOREWARD,
	CAMERA_MOVE_BACKWARD,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT,
	CAMERA_MOVE_UP,
	CAMERA_MOVE_DOWN,
};
class Camera
{
public:
	glm::mat4 view;
	glm::vec3 position, direction, worldUp;
	float speed, yaw, pitch, sensitivity, mouseXLast, mouseYLast;
	bool firstMouseMovement;

	Camera(glm::vec3 startPostion, glm::vec3 startDirection);
	void setPosition(glm::vec3 newPosition);
	void movePosition(CameraMovementType cameraMovementType);
	void setDirection(float mouseXPosition, float mouseYPostion);
	void setViewMatrix();
	glm::mat4 getViewMatrix();

};


#endif
