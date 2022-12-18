#pragma once

#include <glad/gl.h>

#include <vector>

class Model {
public:
	Model(const std::vector<GLfloat> &positions, const std::vector<GLfloat> &uvs, const std::vector<GLuint> &indices, const std::vector<GLfloat> &instancePosition);
	~Model();

	GLuint getVAO() const { return m_vao; }
	GLuint getIndiceCount() const { return m_indiceCount; }

private:
	GLuint m_vao;
	GLuint m_positionVBO;
	GLuint m_uvVBO;
	GLuint m_instanceVbo;
	GLuint m_ebo;

	GLuint m_indiceCount;
};