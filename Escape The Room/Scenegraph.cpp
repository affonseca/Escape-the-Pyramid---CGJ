#include "Scenegraph.h"
#include "Camera.h"

static const std::string defaultNormalMatrixName;

////calculate normal matrix
//Matrix3 view3 = viewMatrix.toMatrix3();
//Matrix3 model3 = projectionMatrix.toMatrix3();
//this->normalMatrix.set((((view3*model3).inverse()).transpose()).toMatrix4());


Scenegraph::Scenegraph() : Scenegraph(NULL, NULL) {}

Scenegraph::Scenegraph(SceneNode* root) : Scenegraph(root, NULL) {}

Scenegraph::Scenegraph(Camera* camera) : Scenegraph(NULL, camera) {}

Scenegraph::Scenegraph(SceneNode* root, Camera* camera) {
	setRoot(root);
	setCamera(camera);
	if (root != NULL) root->setViewCopy(&(this->camera->getViewMatrix()));
}

Scenegraph::~Scenegraph() {
	delete root;
	delete camera;
}

void Scenegraph::setCamera(Camera* camera) {
	this->camera = camera;
	if (camera != NULL) {
		camera->setScenegraph(this);
		camera->updateMatricesInNodes(true, true);
	}
}

Camera* Scenegraph::getCamera() {
	return camera;
}

void Scenegraph::setRoot(SceneNode* root) {
	this->root = root;
	if (root != NULL) {
		root->setParent(NULL);
		if (camera != NULL) {
			camera->updateMatricesInNodes(true, true);
		}
	}
}

void Scenegraph::draw() {
	if (root != NULL) {
		root->setViewCopy(&(this->camera->getViewMatrix()));
		root->draw();
	}
}

SceneNode* Scenegraph::getRoot() {
	return root;
}