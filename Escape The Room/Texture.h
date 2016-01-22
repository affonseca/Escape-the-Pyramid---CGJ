#pragma once
#include <string>
#include "SOIL\src\SOIL.h"
#include "GL\glew.h"
#include "ShaderProgram.h"

/*////////////////////////////////
//			TEXTURE 			//
////////////////////////////////*/

class Texture {
private:
	unsigned char* charStream;
	const char* imageName;
	int width, height;
	GLuint texId;
public:
	Texture(int width, int height, const char* imageName);
	int getWidth();
	int getHeight();
	GLuint getTexId();
	void load(ShaderProgram* shader, GLuint textureUnit);
	void create();
	static void setParams();
	unsigned char* getStream(int width, int height, const char* imageName);
	void freeImage(unsigned char* image);
	std::string nameInShader;
};

