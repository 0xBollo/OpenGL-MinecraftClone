#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;



enum CameraMovement 
{ 
	FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN 
};

enum MovementMode
{
	// The camera movement is exclusively aligned along the axes of the view space
	ALIGNED_TO_VIEW_SPACE,
	// As long as the camera doesn't move up or down, it's stuck on the y-plane of the world space 
	STUCK_ON_WORLD_Y
};

class Camera
{
	public:
		// Camera coordinate system
		glm::vec3 pos;     // origin of the view space (position vector)
		glm::vec3 front;   // z-axis of the view space (normalized direction vector)
		glm::vec3 up;      // y-axis of the view space (normalized direction vector)
		glm::vec3 right;   // x-axis of the view space (normalized direction vector)

		// Euler angles
		float pitch;   // rotation of the camera around the x-axis (look up/down)
		float yaw;     // rotation of the camera around the y-axis (look left/right)

		// Camera options
		MovementMode movementMode;
		float movementSpeed;
		float mouseSensitivity;
		float fov;

		// World coordinate system
		glm::vec3 worldUp;   // y-axis of the world space (normalized direction vector)

		// Constructor
		Camera(
			vec3 pos,
			MovementMode movementMode = ALIGNED_TO_VIEW_SPACE,
			float movementSpeed = 3.5f,
			float mouseSensitivity = 0.02f,
			float fov = 45.0f,
			vec3 worldUp = vec3(0.0f, 1.0f, 0.0f)
		);   

		// Constant member functions
		glm::mat4 getViewMatrix() const;

		// Camera movement
		void move(CameraMovement direction, float deltaTime);
		void lookAround(float x_offset, float y_offset);
		void zoom(float y_offset);

	private:
		// Calculate the normalized direction vectors (front, up, right), which span the camera 
		// coordinate system, based on the current Euler angles
		void calcCoordinateSystem();
};

