#include "Texture.h"

Texture::Texture(int width, int height, const char * imageName)
{
	this->width = width;
	this->height = height;
	this->imageName = imageName;
	this->nameInShader = "tex";
}

int Texture::getWidth()
{
	return this->width;
}

int Texture::getHeight()
{
	return this->height;
}

GLuint Texture::getTexId()
{
	return texId;
}

void Texture::create() {

	unsigned char* stream = NULL;
	if (imageName != NULL)
		stream = this->getStream(width, height, imageName);

	glGenTextures(1, &texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, stream);
	setParams();

	if (imageName != NULL)
		this->freeImage(stream);

	if (imageName != NULL && 0 == stream)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}


}

void Texture::setParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float aniso = 16.0f;
	glGenerateMipmap(GL_TEXTURE_2D);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
}

void Texture::load(ShaderProgram* shader, GLuint textureUnit)
{
	shader->setUniform2DSampler(nameInShader, texId, textureUnit);
}

unsigned char * Texture::getStream(int width, int height, const char* imageName)
{
	return SOIL_load_image(imageName, &width, &height, 0, SOIL_LOAD_RGB);
}

void Texture::freeImage(unsigned char * image)
{
	SOIL_free_image_data(image);
}
