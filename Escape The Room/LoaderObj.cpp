#include "LoaderObj.h"
#include "Mesh.h"

void LoaderObj::parseVertex(std::stringstream & sin)
{
	Vertex v;
	sin >> v.vertex[0] >> v.vertex[1] >> v.vertex[2];
	vertexData.push_back(v);
}

void LoaderObj::parseTexcoord(std::stringstream & sin)
{
	Texcoord t;
	sin >> t.textcoord[0] >> t.textcoord[1];
	texcoordData.push_back(t);
}

void LoaderObj::parseNormal(std::stringstream & sin)
{
	Normal n;
	sin >> n.normal[0] >> n.normal[1] >> n.normal[2];
	normalData.push_back(n);
}

void LoaderObj::parseFace(std::stringstream & sin)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
	}
}

void LoaderObj::parseMaterial(std::stringstream & sin)
{
	MaterialNames m;
	sin >> m.name[0];
	matNamesData.push_back(m);
}

void LoaderObj::parseName(std::stringstream & sin)
{
	sin >> mat.name;
}

void LoaderObj::parseNs(std::stringstream & sin)
{
	sin >> mat.ns;
}

void LoaderObj::parseAmb(std::stringstream & sin)
{
	sin >> mat.amb[0] >> mat.amb[1] >> mat.amb[2];
}

void LoaderObj::parseDiff(std::stringstream & sin)
{
	sin >> mat.dif[0] >> mat.dif[1] >> mat.dif[2];
}

void LoaderObj::parseSpec(std::stringstream & sin)
{
	sin >> mat.spec[0] >> mat.spec[1] >> mat.spec[2];
	materialData.push_back(mat);
}

void LoaderObj::parseNi(std::stringstream & sin)
{
	sin >> mat.ni;
}

void LoaderObj::parseAlpha(std::stringstream & sin)
{
	sin >> mat.alpha;
}

void LoaderObj::parseIllum(std::stringstream & sin)
{
	sin >> mat.illum;
}

void LoaderObj::parseLine(std::stringstream & sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("usemtl") == 0) parseMaterial(sin);
	else if (s.compare("f") == 0) parseFace(sin);
	//mat part
	else if (s.compare("newmtl") == 0) parseName(sin);
	else if (s.compare("Ns") == 0) parseNs(sin);
	else if (s.compare("Ka") == 0) parseAmb(sin);
	else if (s.compare("Kd") == 0) parseDiff(sin);
	else if (s.compare("Ks") == 0) parseSpec(sin);
	else if (s.compare("Ni") == 0) parseNi(sin);
	else if (s.compare("d") == 0) parseAlpha(sin);
	else if (s.compare("illum") == 0) parseIllum(sin);
}

void LoaderObj::loadMeshData(std::string & filename)
{
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
}

void LoaderObj::loadMeshDataMat(std::string & filename, std::string & matfilename)
{
	//read obj file
	std::ifstream ifile1(filename);
	while (ifile1.good()) {
		std::string line;
		std::getline(ifile1, line);
		parseLine(std::stringstream(line));
	}

	//read mtl file
	std::ifstream ifile2(matfilename);
	while (ifile2.good()) {
		std::string line;
		std::getline(ifile2, line);
		parseLine(std::stringstream(line));
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
	MaterialsLoaded = (matNamesData.size() > 0);
}

void LoaderObj::processMeshData()
{
	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		Vertices.push_back(v);
		if (TexcoordsLoaded) {
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			Texcoords.push_back(t);
		}
		if (NormalsLoaded) {
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			Normals.push_back(n);
		}
		if (MaterialsLoaded) {
			for (unsigned int j = 0; j < materialData.size(); j++)
			{
				Mat m = materialData[j];
				Material mtl = Material(Vector3(0.0f, 0.0f, 0.0f), Vector3(m.amb[0], m.amb[1], m.amb[2]), Vector3(m.dif[0], m.dif[1], m.dif[2]), Vector3(m.spec[0], m.spec[1], m.spec[2]), m.ns);
				Materials.push_back(mtl);
			}
		}
	}
}

void LoaderObj::freeMeshData()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
	materialData.clear();
}

GLuint LoaderObj::getNumberOfPolygons()
{
	return Vertices.size();
}

GLuint LoaderObj::getNumberOfTexCoord()
{
	return Texcoords.size();
}

GLuint LoaderObj::getNumberOfNormals()
{
	return Normals.size();
}

GLuint LoaderObj::getVertexSize()
{
	return sizeof(Vertex);
}

GLuint LoaderObj::getTexcoordSize()
{
	return sizeof(Texcoord);
}

GLuint LoaderObj::getNormalSize()
{
	return sizeof(Normal);
}

GLuint LoaderObj::getMaterialSize()
{
	return sizeof(Mat);
}

bool LoaderObj::areTexCoordsLoaded()
{
	return TexcoordsLoaded;
}

bool LoaderObj::areNormalsLoaded()
{
	return NormalsLoaded;
}

std::vector<Vertex> LoaderObj::getVertices()
{
	return Vertices;
}

std::vector<Texcoord> LoaderObj::getTexcoord()
{
	return Texcoords;
}

std::vector<Normal> LoaderObj::getNormal()
{
	return Normals;
}

std::vector<Material> LoaderObj::getMaterial()
{
	return Materials;
}

Material* LoaderObj::getFirstMaterial()
{
	Material* first = NULL;

	if (MaterialsLoaded && Materials.size() > 0)
		first = &(Materials[0]);

	return first;
}

LoaderObj::LoaderObj(std::string & filename, std::string & matfilename)
{
	loadMeshDataMat(filename, matfilename);
	processMeshData();
	freeMeshData();
}

LoaderObj::LoaderObj(std::string & filename)
{
	loadMeshData(filename);
	processMeshData();
	freeMeshData();

	this->colorTexture = NULL;
	this->normalTexture = NULL;
}

std::vector<Vertex> LoaderObj::getPolygons() {
	std::vector<Vertex> res = std::vector<Vertex>((int)Vertices.size());

	for (int i = 0; i < (int)Vertices.size(); i++) {
		res[i].vertex[0] = Vertices[i].vertex[0];
		res[i].vertex[1] = Vertices[i].vertex[1];
		res[i].vertex[2] = Vertices[i].vertex[2];
	}

	return res;

}

void LoaderObj::setColorTexture(Texture * texture)
{
	this->colorTexture = texture;
}

Texture * LoaderObj::getColorTexture()
{
	return this->colorTexture;
}

bool LoaderObj::hasColorTexture()
{
	return (this->colorTexture != NULL);
}

void LoaderObj::setNormalTexture(Texture * texture)
{
	this->normalTexture = texture;
}

Texture * LoaderObj::getNormalTexture()
{
	return this->normalTexture;
}

bool LoaderObj::hasNormalTexture()
{
	return (this->normalTexture != NULL);
}
