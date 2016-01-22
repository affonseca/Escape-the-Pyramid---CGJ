#pragma once
#include <cstring>
#include "LoaderObj.h"
#include "GL\glew.h"
#include "Texture.h"
#include "Material.h"
#include "ColorUtils.h"


typedef struct {
	GLfloat tangent[3];
} Tangent;


typedef struct {
	GLfloat bitangent[3];
} Bitangent;


class Mesh {
public:
	static const GLuint vertexIndex = 0;
	static const GLuint normalIndex = 1;
	static const GLuint textureCoordIndex = 2;
	static const GLuint tangentIndex = 3;
	static const GLuint bitangentIndex = 4;

	Mesh(LoaderObj* loader);
	~Mesh();

	void draw();

protected:
	GLuint vaoID;
	GLuint vertexVBO;
	GLuint textcoorVBO;
	GLuint normalVBO;
	GLuint tangentVBO;
	GLuint bitangentVBO;
	GLuint numberOfPolygons;

	void calcTangentAndBitangent(std::vector<Vertex> vertices, std::vector<Texcoord> texCoords,
		std::vector<Normal> normals, std::vector<Tangent>& tangents, std::vector<Bitangent>& bitangents);
};
