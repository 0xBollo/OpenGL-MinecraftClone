#include "Camera.hpp"



Camera::Camera(vec3 pos, MovementMode movementMode, float movementSpeed, float mouseSensitivity, 
	float fov, vec3 worldUp)
{
	// Set camera coordinate system
	this->pos = pos;
	this->up = vec3(0.0f, 1.0f, 0.0f);

	// Set camera options
	this->movementMode = movementMode;
	this->movementSpeed = movementSpeed;
	this->mouseSensitivity = mouseSensitivity;
	this->fov = fov;

	// Set Euler angles
	this->pitch = 0.0f;
	this->yaw = -90.0f;

	// Set world coordinate system
	this->worldUp = worldUp;

	// Calculate camera coordinate system
	this->calcCoordinateSystem();
}


glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(pos, pos + front, up);   // pos + front = lookAtPoint
}


void Camera::move(CameraMovement direction, float deltaTime)
{
	glm::vec3 moveFront, moveRight, moveUp;

	// Decide which front, up and right vector to use for the movement
	switch (movementMode)
	{
		case ALIGNED_TO_VIEW_SPACE:
			moveFront = front;
			moveRight = right;
			moveUp = up;
			break;
		case STUCK_ON_WORLD_Y:
			moveFront = normalize(vec3(front.x, 0, front.z));
			moveRight = right;
			moveUp = worldUp;
	}

	// The velocity depends on the time elapsed since the last frame
	const float velocity = movementSpeed * deltaTime;

	// Move this camera with the calculated velocity in the calculated direction 
	switch (direction)
	{
		case FORWARD:
			pos += moveFront * velocity;
			break;
		case BACKWARD:
			pos -= moveFront * velocity;
			break;
		case LEFT:
			pos -= moveRight * velocity;
			break;
		case RIGHT:
			pos += moveRight * velocity;
			break;
		case UP:
			pos += moveUp * velocity;
			break;
		case DOWN:
			pos -= moveUp * velocity;
			break;
	}
}


void Camera::lookAround(float x_offset, float y_offset)
{
	// Calculate new Euler angles
	yaw += (mouseSensitivity * x_offset);
	pitch += (mouseSensitivity * (-y_offset));

	// Constraint pitch angle
	if (pitch > 89) 
		pitch = 89;
	else if (pitch < -89) 
		pitch = -89;

	// Calculate the new direction vectors for the camera coordinate system
	// based on the changed Euler angles
	calcCoordinateSystem();
}


void Camera::zoom(float y_offset)
{
	// Calculate new fov
	if (fov <= 45.0f && fov >= 1.0f)
		fov -= y_offset;

	// Constraint fov
	if (fov > 45.0f)
		fov = 45.0f;
	else if (fov < 1.0f)
		fov = 1.0f;
}


void Camera::calcCoordinateSystem()
{
	// Front vector (z-axis)
	glm::vec3 newFront;
	newFront.x = cos(radians(pitch)) * cos(radians(yaw));
	newFront.y = sin(radians(pitch));
	newFront.z = cos(radians(pitch)) * sin(radians(yaw));
	front = normalize(newFront);

	// Right vector (x-axis)
	right = normalize(cross(front, worldUp));

	// Up vector (y-axis)
	up = normalize(cross(right, front));
}