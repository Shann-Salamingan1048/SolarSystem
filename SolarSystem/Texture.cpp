#include"Texture.h"
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum pixelType)
{
	type = texType;
	std::println("Loading texture: {}", image);

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes)
	{
		std::println("Failed to load image: {}", image);
		return;
	}

	GLenum imgFormat;
	switch (numColCh)
	{
	case 1: imgFormat = GL_RED;  break;
	case 4: imgFormat = GL_RGBA; break;
	default:
		imgFormat = GL_RGB;
		std::println("Warning: unexpected numColCh = {}, defaulting to GL_RGB", numColCh);
		break;
	}

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Horizontal wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Vertical

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(texType, 0, imgFormat, widthImg, heightImg, 0, imgFormat, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
}


void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}