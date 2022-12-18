#include "app.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>


void App::init(GLFWwindow *window) {
	ShaderLibrary library;

	m_program = std::make_shared<ShaderProgram>(std::initializer_list<Shader>{
		library.load("assets/shaders/test.vert.spv", ShaderType::VERTEX, "test.vert").value(),
		library.load("assets/shaders/test.frag.spv", ShaderType::FRAGMENT, "test.frag").value()
	});
	m_program->build();

	m_compute = std::make_shared<ShaderProgram>(std::initializer_list<Shader>{
		library.load("assets/shaders/generator.comp.spv", ShaderType::COMPUTE, "generator.comp").value(),
	});
	m_compute->build();

	m_windNoiseTexture = loadImage("assets/textures/noise2.png");
	m_windNoiseTexture->bindTo(1);

	m_projection = glm::perspective(glm::radians(90.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
	
	m_program->bind();
	m_program->uniformMat4(0, m_projection);
	m_program->uniformVec2(4, glm::normalize(glm::vec2(1, -1))); // Wind
	m_program->unbind();

	// Create world
	m_world = std::make_shared<World>(5, 5);
	m_world->generate(m_compute);

	m_camera.installControls(window);
}

void App::update(float delta) {
	glm::mat4 model = glm::mat4(1.0f);

	m_camera.update(delta);

	m_program->bind();
	m_program->uniformMat4(1, m_camera.getMat());
	m_program->uniformMat4(2, model);
	m_program->uniformFloat(3, (float)glfwGetTime());
	m_program->uniformVec3(5, m_camera.getCenter());
	m_program->uniformFloat(6, m_characterPushGrass);
	m_program->unbind();
}

void App::render(float delta) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_program->bind();

	m_world->render(m_camera.getPosition(), m_lodLimit, m_program, Frustum(m_freezeFrustum ? m_frozenFrustum : m_projection * m_camera.getMat()));

	m_program->unbind();
}

void App::renderUI(float delta) {
	static int fpsCounter = 0;
	static int displayedFPS = 0;
	static float fpsTimer = 0.0f;

	++fpsCounter;
	fpsTimer += delta;

	if (fpsTimer > 1.0f) {
		fpsTimer -= 1.0f;
		displayedFPS = fpsCounter;
		fpsCounter = 0;
	}

	if (ImGui::Begin("Statistics")) {
		ImGui::Text("FPS: %d", displayedFPS);
		ImGui::Text("Tiles in view: %d", m_world->getRenderedTileCount());
		ImGui::Text("High tiles: %d", m_world->getRenderedTileCountHigh());
		ImGui::Text("Low tiles: %d", m_world->getRenderedTileCountLow());
		ImGui::Text("Grass blades: %d", m_world->getRenderedTileCount() * m_world->TILE_SIZE * m_world->TILE_SIZE);
		ImGui::Text("Vertices: %d", (m_world->getRenderedTileCountHigh() * 12 + m_world->getRenderedTileCountLow() * 4) * m_world->TILE_SIZE * m_world->TILE_SIZE);
		ImGui::Text("Triangles: %d", (m_world->getRenderedTileCountHigh() * 6 + m_world->getRenderedTileCountLow() * 2) * m_world->TILE_SIZE * m_world->TILE_SIZE);
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Checkbox("Use frustum culling", &m_world->useFrustum);
			ImGui::DragFloat("LOD limit", &m_lodLimit, 1.0f, 0.0f, 200.0f);
		}
		if (ImGui::CollapsingHeader("Debug tools", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Checkbox("Freeze frustum", &m_freezeFrustum)) {
				m_frozenFrustum = m_projection * m_camera.getMat();
			}
			ImGui::Checkbox("Tint LOD", &m_world->debugLOD);
			ImGui::Checkbox("Push grass", &m_characterPushGrass);
		}
	}
	ImGui::End();
}
