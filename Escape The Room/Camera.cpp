#include "Camera.h"


const std::string Camera::defaultViewMatrixName = "View";
const std::string Camera::defaultProjectionMatrixName = "Projection";

Camera::Camera() : Camera(Matrix4(IDENTITY_MATRIX), Matrix4(IDENTITY_MATRIX)) {}

Camera::Camera(Matrix4 projectionMatrix) : Camera(Matrix4(IDENTITY_MATRIX), projectionMatrix) {}

Camera::Camera(Matrix4 viewMatrix, Matrix4 projectionMatrix) {
	this->viewMatrix.set(viewMatrix);
	this->projectionMatrix.set(projectionMatrix);
	updateMatricesInNodes(true, true);

	viewMatrixName = defaultViewMatrixName;
	projectionMatrixName = defaultProjectionMatrixName;
}

void Camera::setViewMatrix(Vector3 eye, Vector3 center, Vector3 up) {
	Vector3 v = (center - eye).getNormalized();
	Vector3 s = (v * up).getNormalized();
	Vector3 u = s*v;

	viewMatrix.set(
		s.x, u.x, -v.x, 0,
		s.y, u.y, -v.y, 0,
		s.z, u.z, -v.z, 0,
		-s.dot(eye), -u.dot(eye), v.dot(eye), 1);

	updateMatricesInNodes(true, false);
}


void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far) {

	projectionMatrix.set(
		2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, -2 / (far - near), 0,
		(left + right) / (left - right), (bottom + top) / (bottom - top), (near + far) / (near - far), 1);

	updateMatricesInNodes(false, true);
}

void Camera::setPerspectiveProjection(float fovY, float aspect, float zNear, float zFar) {
	float theta = fovY / 2;

	float d = 1.0f / tanf(theta);

	projectionMatrix.set(
		d / aspect, 0, 0, 0,
		0, d, 0, 0,
		0, 0, (zNear + zFar) / (zNear - zFar), -1,
		0, 0, (2 * zFar*zNear) / (zNear - zFar), 0);

	updateMatricesInNodes(false, true);
}

void Camera::setViewMatrix(Matrix4 matrix) {
	viewMatrix.set(matrix);
	updateMatricesInNodes(true, false);
}

Matrix4 Camera::getViewMatrix() {
	return viewMatrix;
}

void Camera::setProjectionMatrix(Matrix4 matrix) {
	projectionMatrix.set(matrix);
	updateMatricesInNodes(false, true);
}

Matrix4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

//Uniform matrices names
void Camera::setViewMatrixName(std::string name) {
	viewMatrixName = name;
}

std::string Camera::getViewMatrixName() {
	return viewMatrixName;
}

void Camera::setProjectionMatrixName(std::string name) {
	projectionMatrixName = name;
}

std::string Camera::getProjectionMatrixName() {
	return projectionMatrixName;
}

//scenograph related methods
void Camera::setScenegraph(Scenegraph* scenograph) {
	this->scenegraph = scenograph;
}

Scenegraph* Camera::getScenegraph() {
	return scenegraph;
}

void Camera::updateMatricesInNodes(const bool viewUpdate, const bool projectionUpdate) {
	if (!viewUpdate && !projectionUpdate)
		return;

	if (this->scenegraph == NULL)
		return;

	recurviseUpdateMatricesInNodes(viewUpdate, projectionUpdate, this->scenegraph->getRoot());
	
}

void Camera::recurviseUpdateMatricesInNodes(const bool viewUpdate, const bool projectionUpdate, SceneNode* node) {
	if (node->hasShader()) {
		node->getShader()->begin();
		if (viewUpdate)
			node->getShader()->setUniformMatrix(viewMatrixName, viewMatrix);
		if (projectionUpdate)
			node->getShader()->setUniformMatrix(projectionMatrixName, projectionMatrix);
		node->getShader()->end();
	}

	const std::vector<SceneNode*> children = node->getChildren();
	for (std::vector<SceneNode*>::const_iterator child = children.begin(); child != children.end(); ++child) {
		recurviseUpdateMatricesInNodes(viewUpdate, projectionUpdate, *child);
	}
}