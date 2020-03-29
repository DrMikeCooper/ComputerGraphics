#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, float th, float ph) : position(pos), theta(th), phi(ph) {}

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	void Update(float deltaTime);

	glm::vec3 GetPosition() { return position; }

private:
	float theta;
	float phi;
	glm::vec3 position;

	float lastMouseX;
	float lastMouseY;
};

