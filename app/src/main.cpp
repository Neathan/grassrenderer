#include <glad/gl.h>
#include <glfw/glfw3.h>

#include <spdlog/spdlog.h>

int main() {
	if (!glfwInit()) {
		spdlog::error("Failed to initialize GLFW.");
		return -1;
	}
	
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Application", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		spdlog::error("Failed to initialize OpenGL context");
		return -1;
	}

	glClearColor(0.1, 0.1, 0.1, 1);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
