#pragma once

#include <vector>
#include <string>
#include "GL\glew.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "LightSource.h"

class SceneNode {

private:
	SceneNode* parent;
	std::vector<SceneNode*> children;

	Quaternion rotation = Quaternion(1, 0, 0, 0);
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	GLuint localId;
	Matrix4* viewCopy;

	Mesh* mesh;
	Material* myMaterial;
	Material* myOldMaterial;
	Texture* myTexture;
	Texture* myNormalTexture;
	ShaderProgram* shader;
	Matrix4 rotationMatrix;
	Matrix3 scaleMatrix;
	Matrix4 translationMatrix;
	Matrix4 localMatrix;
	Matrix4 normalMatrix;
	bool reverseTransformationOrder;


	LightSource* myLightSource;

	std::string uniformMatrixName;
	std::string normalMatrixName;

	void updateLocalMatrix();

public:
	static GLuint id;

	static const std::string defaultUniformMatrixName;

	static const std::string defaultNormalMatrixName;

	//Old constructors
	SceneNode();
	SceneNode(bool pickable, Mesh* mesh);
	SceneNode(bool pickable, Mesh* mesh, Material* mat, Texture* tex);
	SceneNode(ShaderProgram* shader);
	SceneNode(LightSource* light);

	//New loaderObj based constructors
	SceneNode(bool pickable, LoaderObj* loaderObj);
	SceneNode(bool pickable, ShaderProgram* shader, LoaderObj* loaderObj);
	SceneNode(bool pickable, LoaderObj* loaderObj, LightSource* light);
	SceneNode(bool pickable, ShaderProgram* shader, LoaderObj* loaderObj, LightSource* light);

	//Compatibility constructor
	SceneNode(bool pickable, ShaderProgram* shader, Mesh* mesh, Material* mat, Texture* tex, Texture* normalTex, LightSource* light);



	~SceneNode();

	void setViewCopy(Matrix4* view);

	//mesh methods
	bool hasMesh();
	Mesh* getMesh();
	void setMesh(Mesh* mesh);

	//shader methods
	bool hasShader();
	bool hasViewCopy();
	bool hasTexture();
	bool hasNormalTexture();
	bool hasMaterial();
	bool isLightSourceShader();
	bool pickable;

	ShaderProgram* getShader();
	void setShader(ShaderProgram* shader);

	//hierarchical methods
	void setParent(SceneNode* parent);
	SceneNode* getParent();

	void addChild(SceneNode* child);
	std::vector<SceneNode*> getChildren();
	SceneNode* getChildAt(unsigned int index);
	SceneNode* getChildById(int id);

	//Transformations
	Quaternion getRotation();
	Vector3 getScale();
	Vector3 getPosition();
	void setRotation(Quaternion rotation); 
	void setScaleX(GLfloat scaleX);
	void setScaleY(GLfloat scaleY);
	void setScaleZ(GLfloat scaleY);
	void setScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	void setScale(GLfloat scaleXYZ);
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat y);
	void setPickable(bool pickable);
	void setMaterial(Material* m);
	void resetMaterial();
	void setLocalMatrix(Matrix4 newMatrix);
	void setReverseTransformationOrder(bool isReverse);
	bool getReverseTransformationOrder();

	//Uniform matrix name
	void setUniformMatrixName(std::string name);
	std::string getUniformMatrixName();
	//Normal matrix name
	void setNormalMatrixName(std::string name);
	std::string getNormalMatrixName();

	void draw();
	void drawFromReferential(Matrix4 position, ShaderProgram* program);

};