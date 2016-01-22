#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Texture.h"
#include "Material.h"
#include "Vector.h"
#include "GL\glew.h"

typedef struct {
	GLfloat vertex[3];
} Vertex;

typedef struct {
	GLfloat textcoord[2];
} Texcoord;

typedef struct {
	GLfloat normal[3];
} Normal;

typedef struct {
	std::string name;
	int illum;
	int texture;
	GLfloat alpha, ns, ni;
	GLfloat dif[3], amb[3], spec[3];
} Mat;

typedef struct {
	std::string name[1];
} MaterialNames;

class LoaderObj
{
private:
	Mat mat;

	std::vector <Vertex> Vertices, vertexData;
	std::vector <Texcoord> Texcoords, texcoordData;
	std::vector <Normal> Normals, normalData;
	std::vector <MaterialNames> matNamesData;
	std::vector <Material> Materials;
	std::vector <Mat> materialData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
	bool TexcoordsLoaded, NormalsLoaded, MaterialsLoaded;

	Texture* colorTexture;
	Texture* normalTexture;

	//obj parse functions
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseMaterial(std::stringstream& sin);
	void parseFace(std::stringstream& sin);

	//mat parse functions
	void parseName(std::stringstream& sin);
	void parseNs(std::stringstream& sin);
	void parseAmb(std::stringstream& sin);
	void parseDiff(std::stringstream& sin);
	void parseSpec(std::stringstream& sin);
	void parseNi(std::stringstream& sin);
	void parseAlpha(std::stringstream& sin);
	void parseIllum(std::stringstream& sin);

	void parseLine(std::stringstream& sin);
	void loadMeshData(std::string& filename);
	void loadMeshDataMat(std::string& filename, std::string& matfilename);
	void processMeshData();
	void freeMeshData();

public:

	LoaderObj(std::string& filename);
	LoaderObj(std::string& filename, std::string& matfilename);

	GLuint getNumberOfPolygons();
	GLuint getNumberOfTexCoord();
	GLuint getNumberOfNormals();
	GLuint getVertexSize();
	GLuint getTexcoordSize();
	GLuint getNormalSize();
	GLuint getMaterialSize();

	bool areTexCoordsLoaded();
	bool areNormalsLoaded();

	std::vector<Vertex> getVertices();
	std::vector<Texcoord> getTexcoord();
	std::vector<Normal> getNormal();
	std::vector<Material> getMaterial();

	Material* getFirstMaterial();

	std::vector<Vertex> getPolygons();

	void setColorTexture(Texture* texture);
	Texture* getColorTexture();
	bool hasColorTexture();

	void setNormalTexture(Texture* texture);
	Texture* getNormalTexture();
	bool hasNormalTexture();

};