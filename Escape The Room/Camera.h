#pragma once

#include <string>

#include "Scenegraph.h"

class Camera
{

public:
	static const std::string defaultViewMatrixName;
	static const std::string defaultProjectionMatrixName;
	Camera();
	Camera(Matrix4 projectionMatrix);
	Camera(Matrix4 viewMatrix, Matrix4 projectionMatrix);

	void setViewMatrix(Vector3 eye, Vector3 center, Vector3 up);
	void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
	void setPerspectiveProjection(float fovY, float aspect, float zNear, float zFar);

	void setViewMatrix(Matrix4 matrix);
	Matrix4 getViewMatrix();
	void setProjectionMatrix(Matrix4 matrix);
	Matrix4 getProjectionMatrix();

	//Uniform matrices names
	void setViewMatrixName(std::string name);
	std::string getViewMatrixName();
	void setProjectionMatrixName(std::string name);
	std::string getProjectionMatrixName();

	//scenograph related methods
	void setScenegraph(Scenegraph* scenegraph);
	Scenegraph* getScenegraph();
	void updateMatricesInNodes(const bool viewUpdate, const bool projectionUpdate);

private:
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;

	std::string viewMatrixName;
	std::string projectionMatrixName;

	Scenegraph* scenegraph;

	//scenograph related methods
	void recurviseUpdateMatricesInNodes(const bool viewUpdate, const bool projectionUpdate, SceneNode* node);

};