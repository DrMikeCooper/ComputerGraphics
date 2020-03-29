#pragma once
#include <glm/glm.hpp>

#include "Shader.h"
#include "OBJMesh.h"

class Scene;

class Instance
{
public:
	Instance(glm::vec3 pos, glm::vec3 angles, glm::vec3 sc, aie::ShaderProgram* shad, aie::OBJMesh* m) :
		position(pos), eulerAngles(angles), scale(sc), shader(shad), mesh(m) {}

	void draw(Scene* scene);

private:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;

	aie::ShaderProgram* shader;
	aie::OBJMesh* mesh;
};

