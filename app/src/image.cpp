#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <spdlog/spdlog.h>

std::shared_ptr<Image> loadImage(const std::string &path) {
	stbi_set_flip_vertically_on_load(true);

	int x, y, comp;
	stbi_uc* data = stbi_load(path.c_str(), &x, &y, &comp, 4);
	
	if (!data) {
		spdlog::error("Failed to load image at: {}", path);
		return nullptr;
	}

	GLuint id;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTextureStorage2D(id, 1, GL_RGBA32F, x, y);
	glTextureSubImage2D(id, 0, 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return std::make_shared<Image>(x, y, id);
}

void Image::bindTo(GLuint binding) {
	glBindTextureUnit(binding, id);
}

void Image::bindToCompute(GLuint binding, GLenum format /*= GL_RGBA32F*/, GLenum access /*= GL_WRITE_ONLY*/) {
	glBindImageTexture(binding, id, 0, GL_FALSE, 0, access, format);
}
