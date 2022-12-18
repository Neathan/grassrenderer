#include "generator.h"

#include <glm/glm.hpp>
#include <glad/gl.h>

#include <vector>


std::shared_ptr<Model> generatePlane(const unsigned int segments, float width, float height,
	const std::vector<GLfloat> &instancePosition) {

	std::vector<GLfloat> positions;
	std::vector<GLfloat> uvs;
	std::vector<GLuint> indices;

	float halfWidth = width / 2.0f;
	float segmentHeight = height / segments;
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

	return std::make_shared<Model>(positions, uvs, indices, instancePosition);
}
