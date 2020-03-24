#include "Camera.h"
#include <glm/ext.hpp>
#include "Input.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

// position ourselves looking down the x-axis at the origin
Camera::Camera() : position(-10, 0, 0), theta(0), phi(0)
{
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);
	glm::vec3 forward(cosf(phiR) * cosf(thetaR), sinf(phiR), cosf(phiR) * sinf(thetaR));
	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f,
		w / h,
		0.1f, 1000.f);
}

void Camera::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);
	//calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cosf(phiR) * cosf(thetaR), sinf(phiR), cosf(phiR) * sinf(thetaR));
	glm::vec3 right(-sinf(thetaR), 0, cosf(thetaR));
	glm::vec3 up(0, 1, 0);

	float speed = 2.0f;

	// use WASD, ZX keys to move camera around
	if (input->isKeyDown(aie::INPUT_KEY_X))
		position -= up * deltaTime * speed;
	if (input->isKeyDown(aie::INPUT_KEY_Z))
		position += up * deltaTime * speed;
	if (input->isKeyDown(aie::INPUT_KEY_A))
		position -= right * deltaTime * speed;
	if (input->isKeyDown(aie::INPUT_KEY_D))
		position += right * deltaTime * speed;
	if (input->isKeyDown(aie::INPUT_KEY_W))
		position += forward * deltaTime * speed;
	if (input->isKeyDown(aie::INPUT_KEY_S))
		position -= forward * deltaTime * speed;

	// get the current mouse coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();
	const float turnSpeed = 0.2f;
	// if the right button is down, increment theta and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		theta += turnSpeed * (mx - lastMouseX);
		phi += turnSpeed * (my - lastMouseY);
	}
	// store this frames values for next frame
	lastMouseX = mx;
	lastMouseY = my;


	// standard linear interpolation
	glm::vec3 p(1,0,0);
	glm::quat r;
	// build a matrix
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
}
