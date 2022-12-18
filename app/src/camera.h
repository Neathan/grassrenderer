#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
	Camera() {}
	Camera(float pitch, float yaw, float distance, glm::vec3 center)
		: m_pitch(pitch), m_yaw(yaw), m_distance(distance), m_targetDistance(distance), m_center(center) {}

	glm::mat4 getMat();
	glm::vec3 getPosition();
	const glm::vec3 &getCenter() const { return m_center; };

	void installControls(GLFWwindow *window);
	void update(float delta);

	void onKeyboardInput(int key, int action);
	void onMouseInput(float x, float y);
	void onScrollInput(float x);
	void onMouseButtonInput(int button, int action);

private:
	bool m_dirty = true;
	glm::mat4 m_cached_view = glm::mat4(1.0f);
	glm::vec3 m_cached_position = glm::vec3(0,0,-5.0f);

	float m_pitch = 0.0f, m_yaw = 0.0f;
	float m_distance = 5.0f;
	float m_targetDistance = 5.0f;
	glm::vec3 m_center = glm::vec3(0);

	glm::vec2 m_input = glm::vec2(0);
	
	float m_mouseX = -1, m_mouseY = -1;
	bool m_mouseDown = false;

	const float SPEED = 80.0f;
	const float VIEW_FACTOR = 0.5f;
	const float SCROLL_FACTOR = 4.0f;
	const float SCROLL_INTERP_SPEED = 5.0f;

	const float MAX_DISTANCE = 100.0f;
	const float MIN_DISTANCE = 1.0f;

};
