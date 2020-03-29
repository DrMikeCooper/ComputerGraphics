#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Scene.h"

class Instance;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_texturedShader;

	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	aie::Texture m_gridTexture;

	aie::OBJMesh m_bunnyMesh;
	aie::OBJMesh m_spearMesh;

	Scene scene;
};