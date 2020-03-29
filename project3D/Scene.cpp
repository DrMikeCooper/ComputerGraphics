#include "Scene.h"
#include "Instance.h"

void Scene::draw()
{
	for (auto it = m_Instances.begin(); it != m_Instances.end(); it++)
	{
		Instance* inst = *it;
		inst->draw(this);
	}

}
