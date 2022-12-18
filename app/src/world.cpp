#include "world.h"

#include <glm/gtc/random.hpp>


void World::generate(const std::shared_ptr<ShaderProgram> &computeShader) {
	if (!m_tiles.empty()) {
		m_tiles.clear();
	}

	m_tiles.reserve(m_width * m_height);

	// TODO: To be replaced by compute shader
	// FastNoise::SmartNode<> generator = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/BwA=");
	const float PADDING = TILE_DIMENSION / TILE_SIZE;

	computeShader->bind();
	computeShader->uniformFloat(1, PADDING);
	computeShader->uniformInt(2, TILE_SIZE);
	computeShader->uniformFloat(3, TILE_DIMENSION);

	// Create tiles
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			// TODO: To be replaced by compute shader
			/*
			std::vector<GLfloat> instanceOffset;
			std::vector<GLfloat> instanceDirection;
			std::vector<GLfloat> instanceTilt;

			instanceOffset.reserve(TILE_SIZE * TILE_SIZE);
			instanceDirection.reserve(TILE_SIZE * TILE_SIZE);

			std::vector<GLfloat> tempHeight;
			instanceTilt.resize(TILE_SIZE * TILE_SIZE);
			tempHeight.resize(TILE_SIZE * TILE_SIZE);

			generator->GenUniformGrid2D(tempHeight.data(), x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, FREQ, 1);
			generator->GenUniformGrid2D(instanceTilt.data(), x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, FREQ, 0);

			for (int zz = 0; zz < TILE_SIZE; ++zz) {
				for (int xx = 0; xx < TILE_SIZE; ++xx) {
					instanceOffset.push_back(x * TILE_DIMENSION + xx * PADDING);
					instanceOffset.push_back(tempHeight.at(xx + zz * TILE_SIZE));
					instanceOffset.push_back(y * TILE_DIMENSION + zz * PADDING);

					glm::vec2 dir = glm::circularRand(1.0f);
					instanceDirection.push_back(dir.x);
					instanceDirection.push_back(dir.y);
				}
			}
			*/

			// Create tile
			GrassData dataHigh = generateGrassData(SEGMENTS_HIGH, BLADE_WIDTH, BLADE_HEIGHT);
			GrassData dataLow = generateGrassData(SEGMENTS_LOW, BLADE_WIDTH, BLADE_HEIGHT);

			// Generate tile data with compute shader
			GLuint offsets;
			GLuint directions;
			GLuint tilts;
			GLuint colors;
			GLuint widths;
			GLuint bends;

			glCreateBuffers(1, &offsets);
			glCreateBuffers(1, &directions);
			glCreateBuffers(1, &tilts);
			glCreateBuffers(1, &colors);
			glCreateBuffers(1, &widths);
			glCreateBuffers(1, &bends);
			
			glNamedBufferData(offsets, TILE_SIZE * TILE_SIZE * 3 * sizeof(GLfloat), nullptr, GL_STATIC_COPY);
			glNamedBufferData(directions, TILE_SIZE * TILE_SIZE * 2 * sizeof(GLfloat), nullptr, GL_STATIC_COPY);
			glNamedBufferData(tilts, TILE_SIZE * TILE_SIZE * sizeof(GLfloat), nullptr, GL_STATIC_COPY);
			glNamedBufferData(colors, TILE_SIZE * TILE_SIZE * 4 * sizeof(GLfloat), nullptr, GL_STATIC_COPY);
			glNamedBufferData(widths, TILE_SIZE * TILE_SIZE * sizeof(GLfloat), nullptr, GL_STATIC_COPY);
			glNamedBufferData(bends, TILE_SIZE * TILE_SIZE * sizeof(GLfloat), nullptr, GL_STATIC_COPY);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, offsets);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, directions);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, tilts);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, colors);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, widths);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, bends);

			computeShader->uniformVec2(0, glm::vec2(x * TILE_DIMENSION, y * TILE_DIMENSION));
			computeShader->dispatch(glm::ceil(TILE_SIZE / 8), glm::ceil(TILE_SIZE / 4), 1);
			

			TileData tileData{ offsets, directions, tilts, colors, widths, bends };

			m_tiles.emplace_back(dataHigh, dataLow, tileData);
		}
	}

	computeShader->unbind();
}

void World::render(const glm::vec3 &cameraPosition, float highLimit, const std::shared_ptr<ShaderProgram>& program, const Frustum &frustum) {
	m_tilesRenderedLastFrameHigh = 0;
	m_tilesRenderedLastFrameLow = 0;

	if (!debugLOD) program->uniformFloat(10, 0.0f);

	program->uniformVec3(11, cameraPosition);

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			const GrassTile &tile = m_tiles.at(x + y * m_width);

			glm::vec3 minP = glm::vec3(x * TILE_DIMENSION, 0, y * TILE_DIMENSION);
			glm::vec3 maxP = glm::vec3(x * TILE_DIMENSION + TILE_DIMENSION, 0, y * TILE_DIMENSION + TILE_DIMENSION);

			if (useFrustum && !frustum.IsBoxVisible(minP, maxP)) {
				continue;
			}

			float dist = glm::distance(cameraPosition, glm::vec3(x * TILE_DIMENSION + TILE_DIMENSION / 2, 0, y * TILE_DIMENSION + TILE_DIMENSION / 2));

			if (dist <= highLimit) {
				++m_tilesRenderedLastFrameHigh;

				if(debugLOD) program->uniformFloat(10, 0.0f);
				glBindVertexArray(tile.getVAOHigh());
				glDrawElementsInstanced(GL_TRIANGLES, tile.getElemCountHigh(), GL_UNSIGNED_INT, nullptr, TILE_SIZE * TILE_SIZE);
			}
			else {
				++m_tilesRenderedLastFrameLow;

				if (debugLOD) program->uniformFloat(10, 0.1f);
				glBindVertexArray(tile.getVAOLow());
				glDrawElementsInstanced(GL_TRIANGLES, tile.getElemCountLow(), GL_UNSIGNED_INT, nullptr, TILE_SIZE * TILE_SIZE);
			}
		}
	}
}
