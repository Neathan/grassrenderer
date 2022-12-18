#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <imgui.h>

#include "app.h"


glm::mat4 Camera::getMat() {
	if (!m_dirty) {
		return m_cached_view;
	}

	m_cached_view = glm::mat4(1.0f);

	m_cached_view = glm::translate(m_cached_view, glm::vec3(m_center));
	m_cached_view = glm::rotate(m_cached_view, m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	m_cached_view = glm::rotate(m_cached_view, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	m_cached_view = glm::translate(m_cached_view, glm::vec3(0.0f, 0.0f, m_distance));

	m_cached_position = glm::vec3(m_cached_view[3]);

	m_cached_view = glm::inverse(m_cached_view);

	m_dirty = false;
	return m_cached_view;
}

glm::vec3 Camera::getPosition() {
	getMat();
	return m_cached_position;
}

void Camera::installControls(GLFWwindow *window) {
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		if (ImGui::GetIO().WantCaptureKeyboard) return;

		App* app = (App*)glfwGetWindowUserPointer(window);
		app->getCamera().onKeyboardInput(key, action);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
		if (ImGui::GetIO().WantCaptureMouse) return;

		App *app = (App *)glfwGetWindowUserPointer(window);
		app->getCamera().onMouseInput((float)x, (float)y);
	});

	glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
		if (ImGui::GetIO().WantCaptureMouse) return;

		App *app = (App *)glfwGetWindowUserPointer(window);
		app->getCamera().onScrollInput((float)y);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
		if (ImGui::GetIO().WantCaptureMouse) return;

		App *app = (App *)glfwGetWindowUserPointer(window);
		app->getCamera().onMouseButtonInput(button, action);
	});
}

void Camera::update(float delta) {
	glm::vec3 forward = glm::rotateY(glm::vec3(0, 0, -1), m_yaw);
	glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

	m_center += (forward * m_input.y + right * m_input.x) * (m_distance / (MAX_DISTANCE - MIN_DISTANCE)) * SPEED * delta;

	// Move towards target distance
	m_distance += (m_targetDistance - m_distance) * SCROLL_INTERP_SPEED * delta;

	m_dirty = true;
}

void Camera::onKeyboardInput(int key, int action) {
	if (key == GLFW_KEY_W) m_input.y += action == GLFW_PRESS ? 1 : action == GLFW_RELEASE ? -1 : 0;
	if (key == GLFW_KEY_S) m_input.y += action == GLFW_PRESS ? -1 : action == GLFW_RELEASE ? 1 : 0;
	if (key == GLFW_KEY_A) m_input.x += action == GLFW_PRESS ? -1 : action == GLFW_RELEASE ? 1 : 0;
	if (key == GLFW_KEY_D) m_input.x += action == GLFW_PRESS ? 1 : action == GLFW_RELEASE ? -1 : 0;
}

void Camera::onMouseInput(float x, float y) {
	if (!m_mouseDown) {
		return;
	}

	// On first input
	if (m_mouseX == -1 && m_mouseY == -1) {
		m_mouseX = x;
		m_mouseY = y;
	}

	float deltaX = x - m_mouseX;
	float deltaY = y - m_mouseY;

	m_mouseX = x;
	m_mouseY = y;

	// Rotate camera
	m_pitch -= glm::radians(deltaY) * VIEW_FACTOR;
	m_yaw -= glm::radians(deltaX) * VIEW_FACTOR;

	m_dirty = true;
}

void Camera::onScrollInput(float x) {
	m_targetDistance -= x * SCROLL_FACTOR;
	m_targetDistance = glm::clamp(m_targetDistance, 1.0f, 100.0f);

	m_dirty = true;
}

void Camera::onMouseButtonInput(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		if (action == GLFW_PRESS) {
			m_mouseDown = true;
		}
		else if (action == GLFW_RELEASE) {
			m_mouseDown = false;
			m_mouseX = -1;
			m_mouseY = -1;
		}
	}
}
