#include "model.h"

Model::Model(const std::vector<GLfloat> &positions, const std::vector<GLfloat> &uvs, const std::vector<GLuint> &indices, const std::vector<GLfloat> &instancePosition) {
	// Generate ids
	glCreateVertexArrays(1, &m_vao);
	glCreateBuffers(1, &m_positionVBO);
	glCreateBuffers(1, &m_uvVBO);
	glCreateBuffers(1, &m_instanceVbo);
	glCreateBuffers(1, &m_ebo);

	// Create storage
	glNamedBufferStorage(m_positionVBO, sizeof(GLfloat) * positions.size(), positions.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_uvVBO, sizeof(GLfloat) * uvs.size(), uvs.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_instanceVbo, sizeof(GLfloat) * instancePosition.size(), instancePosition.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_ebo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
	
	glVertexArrayVertexBuffer(m_vao, 0, m_positionVBO, 0, sizeof(GLfloat) * 3);
	glVertexArrayVertexBuffer(m_vao, 1, m_uvVBO, 0, sizeof(GLfloat) * 2);
	glVertexArrayVertexBuffer(m_vao, 2, m_instanceVbo, 0, sizeof(GLfloat) * 3);
	glVertexArrayElementBuffer(m_vao, m_ebo);

	glEnableVertexArrayAttrib(m_vao, 0);
	glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vao, 0, 0);

	glEnableVertexArrayAttrib(m_vao, 1);
	glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vao, 1, 1);

	glEnableVertexArrayAttrib(m_vao, 2);
	glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vao, 2, 2);
	glVertexArrayBindingDivisor(m_vao, 2, 1);

	m_indiceCount = indices.size();
}

Model::~Model() {
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
	if (m_positionVBO) glDeleteBuffers(1, &m_positionVBO);
	if (m_instanceVbo) glDeleteBuffers(1, &m_instanceVbo);
	if (m_ebo) glDeleteBuffers(1, &m_ebo);
}
