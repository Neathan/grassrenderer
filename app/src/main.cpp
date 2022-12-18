#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <iostream>

#include "app.h"

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, GLchar const *message, void const *user_param);

int main() {
	spdlog::set_level(spdlog::level::trace);

	if (!glfwInit()) {
		spdlog::error("Failed to initialize GLFW.");
		return -1;
	}
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Application", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		spdlog::error("Failed to initialize OpenGL context");
		return -1;
	}

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	glClearColor(0.1, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);


	ImGuiContext *imguiContext = ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	{
		App app{};
		app.init(window);

		glfwSetWindowUserPointer(window, &app);

		// Init ImGui after app, allows us to create callbacks in app that are pass through to by ImGui
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		double last = glfwGetTime();
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			// Calculate delta time
			double current = glfwGetTime();
			float delta = (float)(current - last);
			last = current;

			app.update(delta);
			app.render(delta);


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			app.renderUI(delta);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
		}
	}

	glfwTerminate();
	return 0;
}

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, GLchar const *message, void const *user_param) {

	// Ignore non-significant/non-error codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	auto const srcStr = [source]() {
		switch (source) {
		case GL_DEBUG_SOURCE_API: return "api";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "third party";
		case GL_DEBUG_SOURCE_APPLICATION: return "application";
		case GL_DEBUG_SOURCE_OTHER: return "other";
		default: return "";
		}
	}();

	auto const typeStr = [type]() {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "";
 		}
	}();

	auto const severityStr = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "";
		}
	}();

	spdlog::error("[{}][{}][{}] {}: {}", srcStr, typeStr, severityStr, id, message);
}