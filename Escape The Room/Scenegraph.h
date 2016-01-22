#pragma once

#include "SceneNode.h"

class Camera;

class Scenegraph
{
public:
	Scenegraph();
	Scenegraph(SceneNode* root);
	Scenegraph(Camera* camera);
	Scenegraph(SceneNode* root, Camera* camera);

	~Scenegraph();

	void setCamera(Camera* camera);
	Camera* getCamera();

	void setRoot(SceneNode* root);
	SceneNode* getRoot();

	void draw();

private:
	Camera* camera;
	SceneNode* root;
};