#include "Mesh.h"

Mesh::Mesh(LoaderObj* loader) {

	this->numberOfPolygons = loader->getNumberOfPolygons();

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, loader->getNumberOfPolygons() * loader->getVertexSize(), &loader->getVertices()[0], GL_STATIC_DRAW);

	glVertexAttribPointer(Mesh::vertexIndex, 3, GL_FLOAT, GL_FALSE, loader->getVertexSize(), 0);
	glEnableVertexAttribArray(Mesh::vertexIndex);

	if (loader->areNormalsLoaded()) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, loader->getNumberOfNormals() * loader->getNormalSize(), &loader->getNormal()[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(Mesh::normalIndex);
		glVertexAttribPointer(Mesh::normalIndex, 3, GL_FLOAT, GL_FALSE, loader->getNormalSize(), 0);
	}

	if (loader->areTexCoordsLoaded()) {
		glGenBuffers(1, &textcoorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, textcoorVBO);
		glBufferData(GL_ARRAY_BUFFER, loader->getNumberOfTexCoord() * loader->getTexcoordSize(), &loader->getTexcoord()[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(Mesh::textureCoordIndex);
		glVertexAttribPointer(Mesh::textureCoordIndex, 2, GL_FLOAT, GL_FALSE, loader->getTexcoordSize(), 0);
	}

	if (loader->hasNormalTexture() && loader->areNormalsLoaded() && loader->areTexCoordsLoaded()) {
		std::vector<Tangent> tangents;
		std::vector<Bitangent> bitangents;

		calcTangentAndBitangent(loader->getVertices(), loader->getTexcoord(), loader->getNormal(), tangents, bitangents);

		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Tangent), &tangents[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(Mesh::tangentIndex);
		glVertexAttribPointer(Mesh::tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Tangent), 0);

		glGenBuffers(1, &bitangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
		glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(Bitangent), &bitangents[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(Mesh::bitangentIndex);
		glVertexAttribPointer(Mesh::bitangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Bitangent), 0);

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Mesh::~Mesh() {
	glBindVertexArray(vaoID);
	glDisableVertexAttribArray(Mesh::vertexIndex);
	glDisableVertexAttribArray(Mesh::textureCoordIndex);
	glDisableVertexAttribArray(Mesh::normalIndex);
	glDisableVertexAttribArray(Mesh::tangentIndex);
	glDisableVertexAttribArray(Mesh::bitangentIndex);
	glDeleteBuffers(1, &vertexVBO);
	glDeleteBuffers(1, &textcoorVBO);
	glDeleteBuffers(1, &normalVBO);
	glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &bitangentVBO);
	glDeleteVertexArrays(1, &vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}




void Mesh::draw() {
	glBindVertexArray(vaoID);
	glDrawArrays(GL_TRIANGLES, 0, numberOfPolygons);
	glBindVertexArray(0);
}

void Mesh::calcTangentAndBitangent(std::vector<Vertex> vertices, std::vector<Texcoord> texCoords, std::vector<Normal> normals,
	std::vector<Tangent>& tangents, std::vector<Bitangent>& bitangents)
{
	for (unsigned int i = 0; i<vertices.size(); i += 3) {

		//Getting and converting vertexs and texCoords to Vectors
		Vertex vertex1 = vertices.at(i);
		Vertex vertex2 = vertices.at(i + 1);
		Vertex vertex3 = vertices.at(i + 2);

		Texcoord texCoord1 = texCoords.at(i);
		Texcoord texCoord2 = texCoords.at(i + 1);
		Texcoord texCoord3 = texCoords.at(i + 2);

		Vector3 deltaPos1 = Vector3(vertex2.vertex[0], vertex2.vertex[1], vertex2.vertex[2])
			- Vector3(vertex1.vertex[0], vertex1.vertex[1], vertex1.vertex[2]);
		Vector3 deltaPos2 = Vector3(vertex3.vertex[0], vertex3.vertex[1], vertex3.vertex[2])
			- Vector3(vertex1.vertex[0], vertex1.vertex[1], vertex1.vertex[2]);

		Vector2 deltaUV1 = Vector2(texCoord2.textcoord[0], texCoord2.textcoord[1])
			- Vector2(texCoord1.textcoord[0], texCoord1.textcoord[1]);
		Vector2 deltaUV2 = Vector2(texCoord3.textcoord[0], texCoord3.textcoord[1])
			- Vector2(texCoord1.textcoord[0], texCoord1.textcoord[1]);

		//Current tangent and bitengent calculation
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		Vector3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;


		//processing each vertex
		for (int j = 0; j < 3; j++) {
			Normal tempNormal = normals.at(i + j);
			Vector3 normal = Vector3(tempNormal.normal[0], tempNormal.normal[1], tempNormal.normal[2]);

			//orthogonalizing the tangent
			Vector3 tangentOrtho = (tangent - normal * normal.dot(tangent)).getNormalized();

			//checking and correcting handness
			tangentOrtho *= ((normal*tangent).dot(bitangent) < 0.0f) ? -1.0f : 1.0f;

			//with the tangent ready, calculate the normalized bitengent
			Vector3 bitangentOrtho = normal*tangentOrtho;

			//creating structs and adding to the vectors
			Tangent tangentToAdd = { tangentOrtho.x, tangentOrtho.y , tangentOrtho.z };
			Bitangent bitangentToAdd = { bitangentOrtho.x, bitangentOrtho.y , bitangentOrtho.z };

			tangents.push_back(tangentToAdd);
			bitangents.push_back(bitangentToAdd);

		}

	}
}
