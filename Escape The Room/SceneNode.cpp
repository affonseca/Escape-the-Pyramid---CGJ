#include "SceneNode.h"

const std::string SceneNode::defaultUniformMatrixName = "Model";
const std::string SceneNode::defaultNormalMatrixName = "Normal";
GLuint SceneNode:: id = 0;
SceneNode::SceneNode() : SceneNode(false,NULL, NULL,NULL,NULL,NULL,NULL) {}

SceneNode::SceneNode(bool pickable, Mesh* mesh) : SceneNode(pickable, NULL, mesh, NULL, NULL,NULL,NULL) {}

SceneNode::SceneNode(bool pickable, Mesh* mesh, Material* mat, Texture* tex) : SceneNode(pickable, NULL, mesh, mat, tex,NULL,NULL){}

SceneNode::SceneNode(ShaderProgram* shader) : SceneNode(false, shader, NULL, NULL, NULL,NULL, NULL) {}

SceneNode::SceneNode(LightSource* light) : SceneNode(false, NULL, NULL, NULL, NULL,NULL, light) {}

SceneNode::SceneNode(bool pickable, LoaderObj * loaderObj) : SceneNode(pickable, NULL, loaderObj, NULL) {}

SceneNode::SceneNode(bool pickable, ShaderProgram * shader, LoaderObj * loaderObj) : SceneNode(pickable, shader, loaderObj, NULL) {}

SceneNode::SceneNode(bool pickable, LoaderObj * loaderObj, LightSource * light) : SceneNode(pickable, NULL, loaderObj, light) {}

SceneNode::SceneNode(bool pickable, ShaderProgram * shader, LoaderObj * loaderObj, LightSource * light) :
	SceneNode(pickable, shader, new Mesh(loaderObj), loaderObj->getFirstMaterial(),
		loaderObj->getColorTexture(), loaderObj->getNormalTexture(), light) {}


SceneNode::SceneNode(bool pickable, ShaderProgram* shader, Mesh* mesh, Material* mat, Texture* tex, Texture* normalTex, LightSource* light) {
	
	this->pickable = pickable;

	if (mesh != NULL) {
		++id;
		this->localId = id;
	}
	else this->localId = 0;



	scaleMatrix.set(IDENTITY_MATRIX);
	rotationMatrix.set(IDENTITY_MATRIX);
	translationMatrix.set(IDENTITY_MATRIX);
	localMatrix.set(IDENTITY_MATRIX);


	this->myLightSource = light;
	this->reverseTransformationOrder = false;

	this->myTexture = tex;
	this->myNormalTexture = normalTex;

	this->mesh = mesh;
	this->shader = shader;
	this->myMaterial = mat;
	this->myOldMaterial = mat;

	this->viewCopy = new Matrix4();

	this->reverseTransformationOrder = false;
	

	uniformMatrixName = defaultUniformMatrixName;
	normalMatrixName = defaultNormalMatrixName;

	if (this->hasTexture())
		myTexture->create();

	if (this->hasNormalTexture()) {
		this->myNormalTexture->create();
		this->myNormalTexture->nameInShader = "normalTex";
	}
}


SceneNode::~SceneNode() {
	if (hasMesh())
		delete mesh;
	if (hasShader()) {
		shader->dispose();
		delete shader;
	}
	if (this->hasViewCopy())
		delete viewCopy;
	if (this->hasTexture())
		delete myTexture;
	if (this->hasMaterial())
		delete myMaterial;
	if (this->isLightSourceShader())
		delete myLightSource;
	
	children.clear();
}

void SceneNode::setViewCopy(Matrix4* view)
{

	memcpy(viewCopy, view, sizeof(Matrix4));
	for (std::vector<SceneNode*>::iterator child = children.begin(); child != children.end(); ++child) {
		(*child)->setViewCopy(view);
	}
}

SceneNode* SceneNode::getChildById(int id) {
	if (this->localId == id) return this;
	for (std::vector<SceneNode*>::iterator child = children.begin(); child != children.end(); ++child) {
		SceneNode* aux = (*child)->getChildById(id);
		if (aux != NULL) return aux;
	}
	return NULL;
}

//mesh methods
bool SceneNode::hasMesh() {
	return (mesh != NULL);
}

bool SceneNode::hasMaterial() {
	return (myMaterial != NULL);
}
bool SceneNode::hasTexture() {
	return (myTexture != NULL);
}

Mesh* SceneNode::getMesh() {
	return mesh;
}

void SceneNode::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

//shader methods
bool SceneNode::hasShader() {
	return (shader != NULL);
}

bool SceneNode::isLightSourceShader() {
	return ( this->myLightSource != NULL);
}

bool SceneNode::hasViewCopy() {
	return (viewCopy != NULL);
}

bool SceneNode::hasNormalTexture()
{
	return (this->myNormalTexture != NULL);
}


ShaderProgram* SceneNode::getShader() {
	return shader;
}

void SceneNode::setShader(ShaderProgram* shader) {
	this->shader = shader;
}

//hierarchical methods
void SceneNode::setParent(SceneNode* parent) {
	this->parent = parent;
}

SceneNode* SceneNode::getParent() {
	return this->parent;
}

void SceneNode::addChild(SceneNode* child) {
	this->children.push_back(child);
	child->setParent(this);
}

std::vector<SceneNode*> SceneNode::getChildren() {
	return this->children;
}
SceneNode* SceneNode::getChildAt(unsigned int index) {
	return this->children.at(index);
}

//Transformations
Quaternion SceneNode::getRotation() {
	return this->rotation;
}

Vector3 SceneNode::getScale() {
	return this->scale;
}

Vector3 SceneNode::getPosition() {
	return this->position;
}

void SceneNode::setRotation(Quaternion rotation) {
	this->rotation = rotation;
	rotationMatrix.set(rotation.toMatrix());
	updateLocalMatrix();
}

void SceneNode::setScaleX(GLfloat scaleX) {
	this->scale.x = scaleX;
	scaleMatrix.set(scaleX, 0, 0, 0, scaleMatrix[4], 0, 0, 0, scaleMatrix[8]);
	updateLocalMatrix();
}

void SceneNode::setScaleY(GLfloat scaleY) {
	this->scale.y = scaleY;
	scaleMatrix.set(scaleMatrix[0], 0, 0, 0, scaleY, 0, 0, 0, scaleMatrix[8]);
	updateLocalMatrix();
}

void SceneNode::setScaleZ(GLfloat scaleZ) {
	this->scale.z = scaleZ;
	scaleMatrix.set(scaleMatrix[0], 0, 0, 0, scaleMatrix[4], 0, 0, 0, scaleZ);
	updateLocalMatrix();
}

void SceneNode::setScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ) {
	this->scale.set(scaleX, scaleY, scaleZ);
	scaleMatrix.set(scaleX, 0, 0, 0, scaleY, 0, 0, 0, scaleZ);
	updateLocalMatrix();
}

void SceneNode::setScale(GLfloat scaleXYZ) {
	setScale(scaleXYZ, scaleXYZ, scaleXYZ);
}

void SceneNode::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	this->position.set(x, y, z);
	translationMatrix.getValues()[12] = x;
	translationMatrix.getValues()[13] = y;
	translationMatrix.getValues()[14] = z;
	updateLocalMatrix();
}

void SceneNode::setX(GLfloat x) {
	this->position.x = x;
	translationMatrix.getValues()[12] = x;
	updateLocalMatrix();
}

void SceneNode::setY(GLfloat y) {
	this->position.y = y;
	translationMatrix.getValues()[13] = y;
	updateLocalMatrix();
}

void SceneNode::setZ(GLfloat z) {
	this->position.z = z;
	translationMatrix.getValues()[14] = z;
	updateLocalMatrix();
}

void SceneNode::setPickable(bool pickable)
{
	this->pickable = pickable;
}

void SceneNode::setMaterial(Material* m)
{
	this->myMaterial = m;
}

void SceneNode::resetMaterial()
{
	this->myMaterial = this->myOldMaterial;
}

void SceneNode::updateLocalMatrix() {
	Matrix4 scaleMatrix4 = scaleMatrix.toMatrix4();
	if(this->reverseTransformationOrder)
		localMatrix = scaleMatrix4*rotationMatrix*translationMatrix;
	else
		localMatrix = translationMatrix*rotationMatrix*scaleMatrix4;
}

void SceneNode::setLocalMatrix(Matrix4 matrix) {
	localMatrix = matrix;
}




void SceneNode::setReverseTransformationOrder(bool isReverse) {
	this->reverseTransformationOrder = isReverse;
}


bool SceneNode::getReverseTransformationOrder(){
	return this->reverseTransformationOrder;
}

//Uniform matrix name
void SceneNode::setUniformMatrixName(std::string name) {
	this->uniformMatrixName = name;
}

std::string SceneNode::getUniformMatrixName() {
	return this->uniformMatrixName;
}


//Normal matrix name
void SceneNode::setNormalMatrixName(std::string name) {
	this->normalMatrixName = name;
}

std::string SceneNode::getNormalMatrixName() {
	return this->normalMatrixName;
}

void SceneNode::draw() {
	drawFromReferential(Matrix4(IDENTITY_MATRIX), NULL);
}

void SceneNode::drawFromReferential(Matrix4 position, ShaderProgram* program) {


	Matrix4 globalMatrix = position * this->localMatrix;

	
	if (hasViewCopy())
		this->normalMatrix.set(((*viewCopy).toMatrix3()*globalMatrix.toMatrix3()).inverse().transpose().toMatrix4());
		

	ShaderProgram * currentShader = (hasShader()) ? this->shader : program;

	if (currentShader != NULL) {
		
		currentShader->begin();
		currentShader->setUniformMatrix(uniformMatrixName, globalMatrix);
		currentShader->setUniformMatrix(normalMatrixName, normalMatrix);

		if (this->isLightSourceShader())
			myLightSource->load(globalMatrix,currentShader);
		else{
			if (this->hasMaterial())
				myMaterial->load(currentShader);
			if (this->hasTexture()) {
				myTexture->load(currentShader, 0);
			}
			if (this->hasNormalTexture()) {
				currentShader->setUniformInt("hasNormalMapping", 1);
				myNormalTexture->load(currentShader, 1);
			}
			else {
				currentShader->setUniformInt("hasNormalMapping", 0);
			}
			if (hasMesh()) {
				glStencilFunc(GL_ALWAYS, this->localId + 1, 0xFF);
				mesh->draw();
			}
		}
		currentShader->end();
		currentShader->postProcess();
	}

	for (std::vector<SceneNode*>::iterator child = children.begin(); child != children.end(); ++child) {
		(*child)->drawFromReferential(globalMatrix, currentShader);
	}
}