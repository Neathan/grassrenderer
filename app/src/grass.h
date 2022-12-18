#pragma once

#include <glad/gl.h>

#include <vector>


struct GrassData {
	std::vector<GLfloat> positions;
	std::vector<GLfloat> uvs;
	std::vector<GLuint> indices;
};

struct TileData {
	GLuint offsets;
	GLuint directions;
	GLuint tilts;
	GLuint colors;
	GLuint widths;
	GLuint bends;
};

struct GrassTile {
public:
	GrassTile(const GrassData &highResolutionData,
		const GrassData &lowResolutionData,
		const TileData &tileData);

	~GrassTile();

	GLuint getVAOHigh() const { return m_vaoHigh; }
	GLuint getVAOLow() const { return m_vaoLow; }

	GLuint getElemCountHigh() const { return m_elemCountHigh; }
	GLuint getElemCountLow() const { return m_elemCountLow; }

private:
	GLuint m_vaoHigh;
	GLuint m_vaoLow;

	GLuint m_positionsHigh;
	GLuint m_uvsHigh;
	GLuint m_eboHigh;

	GLuint m_positionsLow;
	GLuint m_uvsLow;
	GLuint m_eboLow;

	TileData m_tileData;

	GLuint m_elemCountHigh;
	GLuint m_elemCountLow;
};

GrassData generateGrassData(const unsigned int segments, float bladeWidth, float bladeHeight);
