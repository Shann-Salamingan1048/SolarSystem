#pragma once
#include "global.h"

enum Cam_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT	
};

constexpr static float YAW = -90.0f;
constexpr static float PITCH = 0.0f;
constexpr static float SPEED = 5.0f;
constexpr static float SENSITIVITY = 0.1f;
constexpr static float ZOOM = 45.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

public:
	// euler Angles
	float Yaw;
	float Pitch;
public:
	float deltaTime = 0.0f;
	float currentFrame = 0.0f;
	float lastFrame = 0.0f;
public:
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

public:
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	glm::mat4 getViewMatrix() const;

	void ProcessKeyboard(Cam_Movement direction);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

public:
	bool enableX_Y_Z[3] = {true, true, true};

private:
	void updateCamVectors();
};