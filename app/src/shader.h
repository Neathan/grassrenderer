#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <optional>


enum class ShaderType {
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};

struct Shader {
	GLuint id;
	ShaderType type;
};

class ShaderLibrary {
public:
	std::optional<Shader> load(const std::string &path, ShaderType type, const std::string &name);

private:
	std::unordered_map<std::string, Shader> m_shaders;
};

class ShaderProgram {
public:
	ShaderProgram(std::initializer_list<Shader> shaders);

	bool build();

	void bind();
	void unbind();

	void dispatch(GLuint x, GLuint y, GLuint z);


	void uniformMat4(GLuint location, const glm::mat4 &value);
	void uniformFloat(GLuint location, float value);
	void uniformInt(GLuint location, int value);
	void uniformVec2(GLuint location, const glm::vec2& value);
	void uniformVec3(GLuint location, const glm::vec3& value);

	GLuint getID() const { return m_id; }

private:
	std::unordered_map<ShaderType, Shader> m_shaders;
	GLuint m_id = 0;
};
