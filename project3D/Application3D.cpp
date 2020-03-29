#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <time.h>

#include <imgui.h>

#include "tiny_obj_loader.h"
#include "Instance.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");
	if (m_shader.link() == false) {
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");
	if (m_texturedShader.link() == false) {
		printf("Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	// initalise the scene;
	scene.camera = new Camera(vec3(-8,11,8), -50, -30);

	// yellow light
	scene.m_light[0].direction = glm::normalize(vec3(1, 1, 1));
	scene.m_light[0].color = glm::vec3(1, 1, 0.5f);
	// second blue light comes from opposite direction but still from above
	scene.m_light[1].direction = glm::normalize(vec3(-1, 1, -1));
	scene.m_light[1].color = glm::vec3(0.5f, 0.5f, 1);

	// define 4 vertices for 2 triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0, 1 }; // bottom left
	vertices[1].texCoord = { 1, 1 }; // bottom right
	vertices[2].texCoord = { 0, 0 }; // top left
	vertices[3].texCoord = { 1, 0 }; // top right

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };
	m_quadMesh.initialise(4, vertices, 6, indices);

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// make the quad 10 units wide
	m_quadTransform = {
	10,0,0,0,
	0,10,0,0,
	0,0,10,0,
	0,0,0,1 };

	// load our models
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false) {
		printf("Bunny Mesh Error!\n");
		return false;
	}

	if (m_spearMesh.load("./soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	// assemble our scene
	scene.m_Instances.push_back(new Instance(vec3(5, 0, 5), vec3(0), vec3(1), &m_shader, &m_spearMesh));
	scene.m_Instances.push_back(new Instance(vec3(-5, 0, 5), vec3(0), vec3(1), &m_texturedShader, &m_spearMesh));
	scene.m_Instances.push_back(new Instance(vec3(5, 0, -5), vec3(0), vec3(1), &m_texturedShader, &m_spearMesh));
	scene.m_Instances.push_back(new Instance(vec3(-5, 0, -5), vec3(0), vec3(1), &m_texturedShader, &m_spearMesh));
	scene.m_Instances.push_back(new Instance(vec3(0, 0, 0), vec3(0), vec3(1), &m_texturedShader, &m_bunnyMesh));

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// query time since application started
	time = 0; // getTime();
	// rotate light

	scene.width = getWindowWidth();
	scene.height = getWindowHeight();

	// update the camera and store its matrices for later use
	scene.camera->Update(deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGui::Begin("Light");
	ImGui::SliderFloat3("Color1", &scene.m_light[0].color[0], 0, 1);
	ImGui::SliderFloat3("Color2", &scene.m_light[1].color[0], 0, 1);
	ImGui::End();
}

void Application3D::draw() {
	// wipe the screen to the background colour
	clearScreen();
	
	// bind shader
	m_texturedShader.bind();

	m_texturedShader.bindUniform("cameraPosition", scene.camera->GetPosition());

	// bind transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	m_texturedShader.bindUniform("diffuseTexture", 0);
	// bind texture to specified location
	m_gridTexture.bind(0);

	// draw quad
	m_quadMesh.draw();

	glm::mat4 camTrans(0);
	const glm::mat4& ref = camTrans;
	vec3 v = glm::normalize(vec3(ref[3]) - vec3(1,1,1));

	// draw the bunny in pink
	//m_shader.bind();

	//m_shader.bindUniform("diffuseTexture", 0);

	// bind transform
	//pvm = m_projectionMatrix * m_viewMatrix * m_bunnyTransform;
	//m_shader.bindUniform("ProjectionViewModel", pvm);
	// draw mesh
	//m_bunnyMesh.draw();

	scene.draw();

	// draw 3D gizmos
	Gizmos::draw(scene.camera->GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * scene.camera->GetViewMatrix());
	// draw 2D gizmos using an orthogonal projection matrix
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	
}