#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "grass.h"
#include "shader.h"

#include "frustum.h"


class World {
public:
	World(int width, int height)
		: m_width(width), m_height(height) {}

	void generate(const std::shared_ptr<ShaderProgram>& computeShader);
	void render(const glm::vec3& cameraPosition, float highLimit, const std::shared_ptr<ShaderProgram> &program, const Frustum& frustum);

	int getRenderedTileCount() const { return m_tilesRenderedLastFrameHigh + m_tilesRenderedLastFrameLow; }
	int getRenderedTileCountHigh() const { return m_tilesRenderedLastFrameHigh; }
	int getRenderedTileCountLow() const { return m_tilesRenderedLastFrameLow; }

	bool debugLOD = false;
	bool useFrustum = true;

	const float SEGMENTS_HIGH = 5;
	const float SEGMENTS_LOW = 1;
	const float BLADE_WIDTH = 1;
	const float BLADE_HEIGHT = 1;

	const float FREQ = 0.02f;

	const int TILE_SIZE = 64;
	const float TILE_DIMENSION = 25.0f;

private:
	int m_width, m_height;

	std::vector<GrassTile> m_tiles;

	int m_tilesRenderedLastFrameHigh = 0;
	int m_tilesRenderedLastFrameLow = 0;
};