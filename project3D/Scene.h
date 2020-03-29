#pragma once
#include <list>
#include "Camera.h"

class Instance;

class Scene
{
public:
	void draw();

	Camera* camera;
	int width, height;

	struct Light {
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light m_light[2];

	std::list<Instance*> m_Instances;
};

