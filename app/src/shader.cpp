#include "shader.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>


std::optional<Shader> ShaderLibrary::load(const std::string &path, ShaderType type, const std::string &name) {
	// Read file
	std::ifstream input(path, std::ios::binary);
	if (input.fail()) {
		spdlog::error("No shader found at: {}", path);
		return {};
	}

	input.seekg(0, std::ios::end); // std::ios::ate does not seem to update tellg return value

	std::vector<char> data(input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(data.data(), data.size());
	
	// Load data
	GLuint id = glCreateShader((GLuint)type);
	glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());
	glSpecializeShader(id, "main", 0, 0, 0);

	// Check for errors
	GLint compileStatus = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		// Log error
		GLint maxLength{};
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		
		std::vector<GLchar> log(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, log.data());

		spdlog::error(log.data());
		glDeleteShader(id);

		return {};
	}

	return std::make_optional<Shader>({ id, type });
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders) {
	for (auto &shader : shaders) {
		m_shaders[shader.type] = shader;
	}
}

bool ShaderProgram::build() {
	// Check if program already built
	if (m_id) {
		// Clean up before build
		glDeleteProgram(m_id);
		// TODO: Evaluate if program should be unbound (has potential unwanted side effects)
	}

	m_id = glCreateProgram();
	for (const auto &[type, shader] : m_shaders) {
		glAttachShader(m_id, shader.id);
	}
	glLinkProgram(m_id);

	// Check for errors
	GLint linkStatus = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		// Log error
		GLint maxLength{};
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> log(maxLength);
		glGetProgramInfoLog(m_id, maxLength, &maxLength, log.data());

		spdlog::error(std::string(log.data()));
		glDeleteProgram(m_id);
		m_id = 0;
		
		return false;
	}

	return true;
}

void ShaderProgram::bind() {
	glUseProgram(m_id);
}

void ShaderProgram::unbind() {
	glUseProgram(0);
}

void ShaderProgram::dispatch(GLuint x, GLuint y, GLuint z) {
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ShaderProgram::uniformMat4(GLuint location, const glm::mat4 &value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::uniformFloat(GLuint location, float value) {
	glUniform1f(location, value);
}

void ShaderProgram::uniformInt(GLuint location, int value) {
	glUniform1i(location, value);
}

void ShaderProgram::uniformVec2(GLuint location, const glm::vec2 &value) {
	glUniform2f(location, value.x, value.y);
}

void ShaderProgram::uniformVec3(GLuint location, const glm::vec3 &value) {
	glUniform3f(location, value.x, value.y, value.z);
}
