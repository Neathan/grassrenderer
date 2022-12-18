#include "grass.h"


GrassTile::GrassTile(const GrassData &highResolutionData,
	const GrassData &lowResolutionData,
	const TileData &tileData)
	: m_tileData(tileData) {

	// Generate ids
	glCreateVertexArrays(1, &m_vaoHigh);
	glCreateVertexArrays(1, &m_vaoLow);

	glCreateBuffers(1, &m_positionsHigh);
	glCreateBuffers(1, &m_uvsHigh);
	glCreateBuffers(1, &m_eboHigh);

	glCreateBuffers(1, &m_positionsLow);
	glCreateBuffers(1, &m_uvsLow);
	glCreateBuffers(1, &m_eboLow);

	// Create storage
	glNamedBufferStorage(m_positionsHigh, sizeof(GLfloat) * highResolutionData.positions.size(), highResolutionData.positions.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_uvsHigh, sizeof(GLfloat) * highResolutionData.uvs.size(), highResolutionData.uvs.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_eboHigh, sizeof(GLuint) * highResolutionData.indices.size(), highResolutionData.indices.data(), GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferStorage(m_positionsLow, sizeof(GLfloat) * lowResolutionData.positions.size(), lowResolutionData.positions.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_uvsLow, sizeof(GLfloat) * lowResolutionData.uvs.size(), lowResolutionData.uvs.data(), GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_eboLow, sizeof(GLuint) * lowResolutionData.indices.size(), lowResolutionData.indices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Configure buffers (HIGH)
	glVertexArrayVertexBuffer(m_vaoHigh, 0, m_positionsHigh, 0, sizeof(GLfloat) * 3);
	glVertexArrayVertexBuffer(m_vaoHigh, 1, m_uvsHigh, 0, sizeof(GLfloat) * 2);
	glVertexArrayElementBuffer(m_vaoHigh, m_eboHigh);

	glVertexArrayVertexBuffer(m_vaoHigh, 2, m_tileData.offsets, 0, sizeof(GLfloat) * 2);
	glVertexArrayVertexBuffer(m_vaoHigh, 3, m_tileData.directions, 0, sizeof(GLfloat) * 2);
	glVertexArrayVertexBuffer(m_vaoHigh, 4, m_tileData.tilts, 0, sizeof(GLfloat));
	glVertexArrayVertexBuffer(m_vaoHigh, 5, m_tileData.colors, 0, sizeof(GLfloat) * 4);
	glVertexArrayVertexBuffer(m_vaoHigh, 6, m_tileData.widths, 0, sizeof(GLfloat));
	glVertexArrayVertexBuffer(m_vaoHigh, 7, m_tileData.bends, 0, sizeof(GLfloat));

	// Configure buffers (LOW)
	glVertexArrayVertexBuffer(m_vaoLow, 0, m_positionsLow, 0, sizeof(GLfloat) * 3);
	glVertexArrayVertexBuffer(m_vaoLow, 1, m_uvsLow, 0, sizeof(GLfloat) * 2);
	glVertexArrayElementBuffer(m_vaoLow, m_eboLow);

	glVertexArrayVertexBuffer(m_vaoLow, 2, m_tileData.offsets, 0, sizeof(GLfloat) * 2);
	glVertexArrayVertexBuffer(m_vaoLow, 3, m_tileData.directions, 0, sizeof(GLfloat) * 2);
	glVertexArrayVertexBuffer(m_vaoLow, 4, m_tileData.tilts, 0, sizeof(GLfloat));
	glVertexArrayVertexBuffer(m_vaoLow, 5, m_tileData.colors, 0, sizeof(GLfloat) * 4);
	glVertexArrayVertexBuffer(m_vaoLow, 6, m_tileData.widths, 0, sizeof(GLfloat));
	glVertexArrayVertexBuffer(m_vaoLow, 7, m_tileData.bends, 0, sizeof(GLfloat));

	// Enable attribute and create bindings (HIGH)
	glEnableVertexArrayAttrib(m_vaoHigh, 0);
	glVertexArrayAttribFormat(m_vaoHigh, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 0, 0);

	glEnableVertexArrayAttrib(m_vaoHigh, 1);
	glVertexArrayAttribFormat(m_vaoHigh, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 1, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 2);
	glVertexArrayAttribFormat(m_vaoHigh, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 2, 2);
	glVertexArrayBindingDivisor(m_vaoHigh, 2, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 3);
	glVertexArrayAttribFormat(m_vaoHigh, 3, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 3, 3);
	glVertexArrayBindingDivisor(m_vaoHigh, 3, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 4);
	glVertexArrayAttribFormat(m_vaoHigh, 4, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 4, 4);
	glVertexArrayBindingDivisor(m_vaoHigh, 4, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 5);
	glVertexArrayAttribFormat(m_vaoHigh, 5, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 5, 5);
	glVertexArrayBindingDivisor(m_vaoHigh, 5, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 6);
	glVertexArrayAttribFormat(m_vaoHigh, 6, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 6, 6);
	glVertexArrayBindingDivisor(m_vaoHigh, 6, 1);

	glEnableVertexArrayAttrib(m_vaoHigh, 7);
	glVertexArrayAttribFormat(m_vaoHigh, 7, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoHigh, 7, 7);
	glVertexArrayBindingDivisor(m_vaoHigh, 7, 1);

	// Enable attribute and create bindings (LOW)
	glEnableVertexArrayAttrib(m_vaoLow, 0);
	glVertexArrayAttribFormat(m_vaoLow, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 0, 0);

	glEnableVertexArrayAttrib(m_vaoLow, 1);
	glVertexArrayAttribFormat(m_vaoLow, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 1, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 2);
	glVertexArrayAttribFormat(m_vaoLow, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 2, 2);
	glVertexArrayBindingDivisor(m_vaoLow, 2, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 3);
	glVertexArrayAttribFormat(m_vaoLow, 3, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 3, 3);
	glVertexArrayBindingDivisor(m_vaoLow, 3, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 4);
	glVertexArrayAttribFormat(m_vaoLow, 4, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 4, 4);
	glVertexArrayBindingDivisor(m_vaoLow, 4, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 5);
	glVertexArrayAttribFormat(m_vaoLow, 5, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 5, 5);
	glVertexArrayBindingDivisor(m_vaoLow, 5, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 6);
	glVertexArrayAttribFormat(m_vaoLow, 6, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 6, 6);
	glVertexArrayBindingDivisor(m_vaoLow, 6, 1);

	glEnableVertexArrayAttrib(m_vaoLow, 7);
	glVertexArrayAttribFormat(m_vaoLow, 7, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vaoLow, 7, 7);
	glVertexArrayBindingDivisor(m_vaoLow, 7, 1);

	m_elemCountHigh = highResolutionData.indices.size();
	m_elemCountLow = lowResolutionData.indices.size();
}

GrassTile::~GrassTile() {
	if (m_vaoHigh) glDeleteVertexArrays(1, &m_vaoHigh);
	if (m_positionsHigh) glDeleteBuffers(1, &m_positionsHigh);
	if (m_uvsHigh) glDeleteBuffers(1, &m_uvsHigh);
	if (m_eboHigh) glDeleteBuffers(1, &m_eboHigh);

	if (m_vaoLow) glDeleteVertexArrays(1, &m_vaoLow);
	if (m_positionsLow) glDeleteBuffers(1, &m_positionsLow);
	if (m_uvsLow) glDeleteBuffers(1, &m_uvsLow);
	if (m_eboLow) glDeleteBuffers(1, &m_eboLow);

	if (m_tileData.offsets) glDeleteBuffers(1, &m_tileData.offsets);
	if (m_tileData.directions) glDeleteBuffers(1, &m_tileData.directions);
	if (m_tileData.tilts) glDeleteBuffers(1, &m_tileData.tilts);
	if (m_tileData.colors) glDeleteBuffers(1, &m_tileData.colors);
}

GrassData generateGrassData(const unsigned int segments, float bladeWidth, float bladeHeight) {
	std::vector<GLfloat> positions;
	std::vector<GLfloat> uvs;
	std::vector<GLuint> indices;

	float halfWidth = bladeWidth / 2.0f;
	float segmentHeight = bladeHeight / segments;
	float segmentUVIncrement = 1.0f / segments;

	for (unsigned int segment = 0; segment < segments + 1; ++segment) {
		positions.push_back(-halfWidth);
		positions.push_back(segment * segmentHeight);
		positions.push_back(0.0f);

		positions.push_back(halfWidth);
		positions.push_back(segment * segmentHeight);
		positions.push_back(0.0f);

		uvs.push_back(0);
		uvs.push_back(segment * segmentUVIncrement);

		uvs.push_back(1);
		uvs.push_back(segment * segmentUVIncrement);
	}

	for (unsigned int segment = 0; segment < segments; ++segment) {
		indices.push_back(0 + segment * 2);
		indices.push_back(1 + segment * 2);
		indices.push_back(2 + segment * 2);

		indices.push_back(1 + segment * 2);
		indices.push_back(3 + segment * 2);
		indices.push_back(2 + segment * 2);
	}

	return GrassData{ positions, uvs, indices };
}


