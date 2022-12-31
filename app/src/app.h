#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>

#include "shader.h"
#include "camera.h"
#include "image.h"

#include "world.h"


class App {
public:
	void init(GLFWwindow *window);
	void update(float delta);
	void render(float delta);
	void renderUI(float delta);

	Camera &getCamera() { return m_camera; }

private:
	glm::mat4 m_projection = glm::mat4(1.0f);

	std::shared_ptr<ShaderProgram> m_program = nullptr;
	std::shared_ptr<ShaderProgram> m_compute = nullptr;


	std::shared_ptr<Image> m_windNoiseTexture = nullptr;

	std::shared_ptr<World> m_world = nullptr;

	Camera m_camera{glm::radians(-40.0f), glm::radians(45.0f), 15.0f, glm::vec3(125.0f, 1.0f, 125.0f)};

	// Settings
	float m_lodLimit = 125.0f;
	bool m_characterPushGrass = false;
	bool m_freezeFrustum = false;
	glm::mat4 m_frozenFrustum = glm::mat4(1.0f);

};
