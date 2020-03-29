#include "Instance.h"
#include "Instance.h"
#include "Scene.h"

#include <glm/ext.hpp>


void Instance::draw(Scene* scene)
{
	shader->bind();


	float rad = 6.28f / 360.0f;
	glm::mat4 transform = glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), eulerAngles.z * rad, glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), eulerAngles.y * rad, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), eulerAngles.x * rad, glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);

	// bind transform
	auto pvm = scene->camera->GetProjectionMatrix(scene->width, scene->height) * scene->camera->GetViewMatrix() * transform;
	shader->bindUniform("ProjectionViewModel", pvm);
	shader->bindUniform("ModelMatrix", transform);
	// bind transforms for lighting
	shader->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(transform)));

	// copy our light info into primitve arrays
	glm::vec3 lightDirections[2];
	glm::vec3 lightColors[2];
	for (int i = 0; i < 2; i++)
	{
		lightDirections[i] = scene->m_light[i].direction;
		lightColors[i] = scene->m_light[i].color;
	}

	// pass the arrays through
	shader->bindUniform("lightDirection", 2, &lightDirections[0]);
	shader->bindUniform("lightColor", 2, &lightColors[0]);

	// draw mesh
	mesh->draw();
}
