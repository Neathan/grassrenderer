#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>

class Image {
public:
	Image(int width, int height, GLuint id) : width(width), height(width), id(id) {}

	void bindTo(GLuint binding);
	void bindToCompute(GLuint binding, GLenum format = GL_RGBA32F, GLenum access = GL_WRITE_ONLY);


public:
	const int width;
	const int height;
	const GLuint id;
};


std::shared_ptr<Image> loadImage(const std::string &path);
