#pragma once

#include <memory>

#include "model.h"


std::shared_ptr<Model> generatePlane(const unsigned int segments, float width, float height, const std::vector<GLfloat> &instancePosition);
