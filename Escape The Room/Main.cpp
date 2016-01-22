///////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Modern OpenGL.
// A "hello world" of Modern OpenGL.
//
// (c) 2013-15 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "LoaderObj.h"
#include "Tests.h"
#include "Scenegraph.h"
#include "SnapshotMaker.h"
#include "Camera.h"
#include "Timeline.h"
#include "KeyManager.h"
#include "LightSource.h"
#include "Material.h"
#include "GameManager.h"

#define CAPTION "Escape the Pyramid"
#define PI		3.14159265
#define SQRT_2	1.41421356

int WinX = 1280, WinY = 720; //WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0, oldTimeSinceStart;

GLbyte color[4];
GLfloat depth;
GLuint index;
GLuint id;
GLuint objPicked;
Vector3 cameraPos1;

Scenegraph* scenegraph;
SceneNode *rootNode;
MultiPassShaderProgram* shaderProgram;
Timeline * timeline;

KeyManager keyManager;
float proximity = 3.5f;
float forwardAndbacward = 0.0f;
float upAndDown = 0.0f;
Vector2 viewAngle(0, 0), angleDifference(0, 0), tablePosition(0, 0);
Quaternion viewQuaternion = Quaternion((GLfloat)PI / 3.5f, Vector3(1.0f, 0.0f, 0.0f));
Vector3 rootPosition(0.0f, -4.0f, 0.0f), tangramPosition(0.0f, 0.0f, 0.0f);
Vector2 mousePosition;
bool eyeMode = false;
bool quaternianMode = false;
bool inPrespective = true, inMovement = false;

/////////////////////////////////////////////////////////////////////// LERPERSs
void lerpRotation(SceneNode* target, float* first, float* second, float percentage) {
	Quaternion firstQ = Quaternion(first[0], Vector3(first[1], first[2], first[3]));
	Quaternion secondQ = Quaternion(second[0], Vector3(second[1], second[2], second[3]));
	target->setRotation(firstQ.slerp(secondQ, percentage));
}

void lerpPosition(SceneNode* target, float* first, float* second, float percentage) {
	Vector3 firstP = Vector3(first[0], first[1], first[2]);
	Vector3 secondP = Vector3(second[0], second[1], second[2]);
	Vector3 finalP = firstP.lerp(secondP, percentage);
	target->setPosition(finalP.x, finalP.y, finalP.z);
}

/////////////////////////////////////////////////////////////////////// ERRORs

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

ShaderProgram* createShaderProgram()
{

	ShaderProgram * outlinerProgram = new ShaderProgram();
	outlinerProgram->addAttribute("in_ScreenCoord", MultiPassShaderProgram::screenCoordIndex);
	outlinerProgram->addUniform("LineWidth");
	outlinerProgram->addUniform("WindowResolution");
	outlinerProgram->addUniform("ColorTexture");
	outlinerProgram->addUniform("NormalTexture");
	outlinerProgram->create("../shaders/OutlineVertexShader.hlsl", "../shaders/OutlineFragmentShader.hlsl");

	std::string gotError = outlinerProgram->getCompilationResult();
	if (!gotError.empty()) {
		std::cerr << gotError << std::endl << "ERROR: Could not create shaders." << std::endl;
		exit(EXIT_FAILURE);
	}

	checkOpenGLError("ERROR: Could not create shaders.");

	outlinerProgram->begin();
	outlinerProgram->setUniformFloat("LineWidth", 1.5f);
	outlinerProgram->end();

	shaderProgram = new MultiPassShaderProgram(Vector2((float)WinX, (float)WinY));
	shaderProgram->addOutputTexture("ColorTexture");
	shaderProgram->addOutputTexture("NormalTexture");

	checkOpenGLError("ERROR: Could not create shaders.");
	shaderProgram->addAttribute("in_Position", Mesh::vertexIndex);
	shaderProgram->addAttribute("in_Normal", Mesh::normalIndex);
	shaderProgram->addAttribute("in_TextureCoord", Mesh::textureCoordIndex);
	shaderProgram->addUniform(SceneNode::defaultUniformMatrixName);
	shaderProgram->addUniform(SceneNode::defaultNormalMatrixName);
	shaderProgram->addUniform(Camera::defaultViewMatrixName);
	shaderProgram->addUniform(Camera::defaultProjectionMatrixName);

	//Light stuff
	shaderProgram->addUniform("LightPosition");
	shaderProgram->addUniform("LightAmbientColor");
	shaderProgram->addUniform("LightDiffuseColor");
	shaderProgram->addUniform("LightSpecularColor");
	shaderProgram->addUniform("LightRange");
	shaderProgram->addUniform("LightAttenuation");

	//Material stuff
	shaderProgram->addUniform("MaterialEmissiveColor");
	shaderProgram->addUniform("MaterialAmbientColor");
	shaderProgram->addUniform("MaterialDiffuseColor");
	shaderProgram->addUniform("MaterialSpecularColor");
	shaderProgram->addUniform("MaterialShininess");

	shaderProgram->addUniform("tex");
	shaderProgram->addUniform("normalTex");
	shaderProgram->addUniform("hasNormalMapping");

	shaderProgram->create("../shaders/VertexShader.hlsl", "../shaders/FragmentShader.hlsl", outlinerProgram);

	gotError = shaderProgram->getCompilationResult();
	if (!gotError.empty()) {
		std::cerr << gotError << std::endl << "ERROR: Could not create shaders." << std::endl;
		exit(EXIT_FAILURE);
	}
	checkOpenGLError("ERROR: Could not create shaders.");

	return shaderProgram;
}

/////////////////////////////////////////////////////////////////////// SCENE

SceneNode* keyNode;
SceneNode* keyNode2;
SceneNode* keyNode3;
SceneNode* keyNode4;
SceneNode* doorNode;

void createScene()
{
	ShaderProgram* currentShaderProgram = createShaderProgram();

	LightSource* torch1=  new  LightSource(Vector3(-29.0f, 15.0f, -30.0f),
											Vector3(0.5f, 0.5f, 0.5f),
											Vector3(1.0f, 0.25f, 0.0f),
											Vector3(1.0f, 0.25f, 0.0f),
											50.0f, 
											Vector3(0.45f, 0.02f, 0.01f),
											1);

	LightSource* torch2 = new  LightSource(Vector3(-4.0f, 15.0f, -30.0f),
											Vector3(0.5f, 0.5f, 0.5f),
											Vector3(1.0f, 0.25f, 0.0f),
											Vector3(1.0f, 0.25f, 0.0f),
											50.0f,
											Vector3(0.45f, 0.02f, 0.01f),
											2);
	
	LightSource* torch3 = new  LightSource(Vector3(-29.0f, 15.0f, 32.0f),
											Vector3(0.5f, 0.5f, 0.5f),
											Vector3(1.0f, 0.25f, 0.0f),
											Vector3(1.0f, 0.25f, 0.0f),
											50.0f,
											Vector3(0.45f, 0.02f, 0.01f),
											3);
	LightSource* torch4 = new  LightSource(Vector3(-4.0f, 15.0f, 32.0f),
											Vector3(0.5f, 0.5f, 0.5f),
											Vector3(1.0f, 0.25f, 0.0f),
											Vector3(1.0f, 0.25f, 0.0f),
											50.0f,
											Vector3(0.45f, 0.02f, 0.01f),
											4);

	Material*  chrome =  new  Material(Vector3(0.0f, 0.0f, 0.0f),
									   Vector3(0.25f, 0.25f, 0.25f),
									   Vector3(0.4f, 0.4f, 0.4f),
									   Vector3(0.774597f, 0.774597f, 0.774597f),
									   76.8f);

	
	Material*  human = new  Material(Vector3(0.0f, 0.0f, 0.0f),
											Vector3(0.5f, 0.5f, 0.5f),
											Vector3(1.0f, 1.0f, 1.0f),
											Vector3(0.0f, 0.0f, 0.0f),
											0.0f);

	Texture*  egyptTex = new Texture(800, 531, "../textures/egypt.jpg");
	Texture*  egyptNormalTex = new Texture(800, 531, "../textures/egypt_normal.jpg");

	Texture*  woodTex = new Texture(1632, 1224, "../textures/wood.jpg");
	Texture*  woodNormalTex = new Texture(1632, 1224, "../textures/wood_normal.jpg");

	Texture*  sandTex = new Texture(512, 512, "../textures/sand.jpg");
	Texture*  sandNormalTex = new Texture(512, 512, "../textures/sand_normal.jpg");

	Texture*  laraTex = new Texture(1024, 1024, "../textures/lara.png");
	Texture*  laraNormalTex = new Texture(1024, 1024, "../textures/lara_normal.png");
	


	Texture*  doorTex = new Texture(800, 531, "../textures/egypt.jpg");

	Texture*  keyTex = new Texture(512, 512, "../textures/Aluminum.jpg");

	//Load obj from file
	std::string roomFile = "../model/EscapeTheRoom.obj";
	std::string roomMTLFile = "../materials/EscapeTheRoom.mtl";

	std::string roofFile = "../model/roof.obj";
	std::string roofMTLFile = "../materials/roof.mtl";

	std::string doorFile = "../model/door.obj";
	std::string doorMTLFile = "../materials/door.mtl";

	std::string keyFile = "../model/key.obj";
	std::string keyMTLFile = "../materials/key.mtl";

	std::string laraFile = "../model/lara.obj";
	std::string laraMTLFile = "../materials/lara.mtl";

	std::string chestFile = "../model/chest.obj";
	std::string chestMTLFile = "../materials/chest.mtl";

	LoaderObj* roomLoader = new LoaderObj(roomFile, roomMTLFile);
	roomLoader->setColorTexture(egyptTex);
	roomLoader->setNormalTexture(egyptNormalTex);

	LoaderObj* chestLoader = new LoaderObj(chestFile, chestMTLFile);
	chestLoader->setColorTexture(woodTex);
	chestLoader->setNormalTexture(woodNormalTex);

	LoaderObj* roofLoader = new LoaderObj(roofFile, roofMTLFile);
	roofLoader->setColorTexture(sandTex);
	roofLoader->setNormalTexture(sandNormalTex);

	LoaderObj* keyLoader = new LoaderObj(keyFile,keyMTLFile);
	keyLoader->setColorTexture(keyTex);

	LoaderObj* doorLoader = new LoaderObj(doorFile);
	doorLoader->setColorTexture(doorTex);
	doorLoader->setNormalTexture(egyptNormalTex);

	LoaderObj* laraLoader = new LoaderObj(laraFile);
	laraLoader->setColorTexture(laraTex);
	//laraLoader->setNormalTexture(laraNormalTex);

	Camera* camera = new Camera();
	camera->setPerspectiveProjection(((float)PI / 2.0f), (float)WinX / WinY, 0.1f, 100);

	//The table has a different position and is lower than the center
	rootNode = new SceneNode(currentShaderProgram);
	rootNode->setY(-1.0f);

	SceneNode* mainNode = new SceneNode();

	SceneNode* models = new SceneNode();
	SceneNode* lights = new SceneNode();

	SceneNode* torch1Node = new SceneNode(torch1);
	SceneNode* torch2Node = new SceneNode(torch2);
	SceneNode* torch3Node = new SceneNode(torch3);
	SceneNode* torch4Node = new SceneNode(torch4);

	SceneNode* roomNode = new SceneNode(false,roomLoader);
	roomNode->setPosition(0, -2, 0);
	
	SceneNode* chestNode = new SceneNode(false, chestLoader);
	chestNode->setPosition(1, -2, 1);
	chestNode->setScale(2);

	SceneNode* roofNode = new SceneNode(false, roofLoader);
	roofNode->setPosition(-45, -2, 50);

	SceneNode* groundNode = new SceneNode(false, roofLoader);
	groundNode->setPosition(-45, -23, 50);

	doorNode = new SceneNode(false, doorLoader);
	doorNode->setPosition(-1.2, -2, 0);


	keyNode = new SceneNode(true, keyLoader);
	keyNode->setScale(5);
	keyNode->setPosition(0, 2, 0);

	keyNode2 = new SceneNode(true, keyLoader);
	keyNode2->setScale(5);
	keyNode2->setPosition(-38, 2, 40);

	keyNode3 = new SceneNode(true, keyLoader);
	keyNode3->setScale(2);
	keyNode3->setPosition(-39, 2, -28);

	keyNode4 = new SceneNode(true, keyLoader);
	keyNode4->setScale(2);
	keyNode4->setPosition(-39, 2, 30);

	SceneNode* laraNode = new SceneNode(true, laraLoader);
	laraNode->setScale(2.5f);
	laraNode->setPosition(-29.0f, 15.0f, 32.0f);

	SceneNode* laraNode2 = new SceneNode(true, laraLoader);
	laraNode2->setScale(5.5f);
	laraNode2->setPosition(-0.0f, 1.0f, 0.0f);

	SceneNode* laraNode3 = new SceneNode(true, laraLoader);
	laraNode3->setScale(1.5f);
	laraNode3->setPosition(-3.0f, 1.0f, 0.0f);

	SceneNode* laraNode4 = new SceneNode(true, laraLoader);
	laraNode4->setScale(1.0f);
	laraNode4->setPosition(-9.0f, 1.0f, 9.0f);

	rootNode->addChild(mainNode);
	mainNode->addChild(models);
	mainNode->addChild(lights);
	models->addChild(roomNode);
	models->addChild(doorNode);
	models->addChild(roofNode);
	models->addChild(keyNode);
	models->addChild(keyNode2);
	models->addChild(keyNode3);
	models->addChild(keyNode4);

	models->addChild(groundNode);
	models->addChild(chestNode);
	/*models->addChild(laraNode);
	models->addChild(laraNode2);
	models->addChild(laraNode3);
	models->addChild(laraNode4);*/
	lights->addChild(torch1Node);
	lights->addChild(torch2Node);
	lights->addChild(torch3Node);
	lights->addChild(torch4Node);
	
	scenegraph = new Scenegraph(rootNode, camera);


	//creating the timeline
	timeline = new Timeline();
	

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyScene()
{
	timeline->stop();
	delete timeline;
	delete scenegraph;
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// DRAW


SnapshotMaker snapshotMaker = SnapshotMaker();
float walkFactor=0;
Matrix4 walkLeftRot = Matrix::createRotationMatrix(0, PI / 8, 0);
bool picked=false;
GameManager gameManager = GameManager();
void act(float delta) {
	

	std::vector<Vector3> positions = std::vector<Vector3>(4);
	positions[0] = keyNode->getPosition();
	positions[1] = keyNode2->getPosition();
	positions[2] = keyNode3->getPosition();
	positions[3] = keyNode4->getPosition();
	gameManager.updatePositions(positions);
	
	

	if (keyManager.isNormalPressed('j')) {
		upAndDown += (cos(walkFactor)*50)*delta/2;

		if (walkFactor > -PI)
			walkFactor += 10 * delta/2;
		else
		{
			walkFactor = 0;
			upAndDown = 0;
			return;
			
		}
	}
	if (keyManager.isNormalPressed(27)) {
		exit(EXIT_SUCCESS);
	}

	if (keyManager.isSpecialPressed(GLUT_KEY_LEFT) || keyManager.isNormalPressed('a'))
	{
		

		if (!gameManager.checkPlayerPosition(rootPosition)) {
			rootPosition.x += cos(viewAngle.x)* 30.5f * delta;
			rootPosition.z += sin(viewAngle.x)* 30.5f * delta;
			return;
		}
		upAndDown += (cos(walkFactor) * 5)*delta;

		rootPosition.z += sin(viewAngle.x)* 10.5f * delta;
		rootPosition.x += cos(viewAngle.x)* 10.5f * delta;



		if (walkFactor <= 2 * PI)
			walkFactor += 10 * delta;
		else
		{
			walkFactor = 0;
			upAndDown = 0;
		}

	}
	if (keyManager.isSpecialPressed(GLUT_KEY_RIGHT) || keyManager.isNormalPressed('d'))
	{
		if (!gameManager.checkPlayerPosition(rootPosition)) {
			rootPosition.x -= cos(viewAngle.x)* 30.5f * delta;
			rootPosition.z -= sin(viewAngle.x)* 30.5f * delta;
			return;
		}
		upAndDown += (cos(walkFactor) * 5)*delta;

		rootPosition.z -= sin(viewAngle.x)* 10.5f * delta;
		rootPosition.x -= cos(viewAngle.x)* 10.5f * delta;

		

		if (walkFactor <= 2 * PI)
			walkFactor += 10 * delta;
		else
		{
			walkFactor = 0;
			upAndDown = 0;
		}

	}
	if (keyManager.isSpecialPressed(GLUT_KEY_DOWN) || keyManager.isNormalPressed('s'))
	{
		
		if (!gameManager.checkPlayerPosition(rootPosition)) {
			rootPosition.x -= sin(viewAngle.x)* 30.5f * delta;
			rootPosition.z += cos(viewAngle.x)* 30.5f * delta;
			return;
		}
		upAndDown += (cos(walkFactor)*5)*delta;
		rootPosition.x += sin(viewAngle.x)* 10.5f * delta;
		rootPosition.z -= cos(viewAngle.x)* 10.5f * delta;


		if (walkFactor <= 2 * PI)
			walkFactor += 10 * delta;
		else
		{
			walkFactor = 0;
			upAndDown = 0;
		}

	}
	if (keyManager.isSpecialPressed(GLUT_KEY_UP) || keyManager.isNormalPressed('w'))
	{ 
		if (!gameManager.checkPlayerPosition(rootPosition)) {
			rootPosition.x += sin(viewAngle.x)* 30.5f * delta;
			rootPosition.z -= cos(viewAngle.x)* 30.5f * delta;
			return;
		}

		upAndDown += (cos(walkFactor)*5)*delta;
		rootPosition.x -= sin(viewAngle.x)* 10.5f * delta;
		rootPosition.z += cos(viewAngle.x)* 10.5f * delta;


		if (walkFactor <= 2 * PI)
			walkFactor += 10 * delta;
		else
		{
			walkFactor = 0;
			upAndDown = 0;
		}
		
	}
	

	if (keyManager.isNormalPressed(' ')) snapshotMaker.takePhoto(WinX, WinY);

	timeline->update(delta);
	rootNode->setPosition(rootPosition.x, rootPosition.y, rootPosition.z);

	
	
}

void changeView() {


	if (viewAngle.y > 9 * PI / 4) viewAngle.y = 9 * PI / 4;
	if (viewAngle.y < 7 * PI / 4) viewAngle.y = 7 * PI / 4;

	if (quaternianMode) {
		Matrix4 viewMatrix;
		Quaternion newQuaternion(angleDifference.getNorm(), Vector3(angleDifference.y, angleDifference.x, 0.0f));
		viewQuaternion = newQuaternion*viewQuaternion;
		viewMatrix = Matrix::createTranslationMatrix(forwardAndbacward, upAndDown, -proximity)*viewQuaternion.toMatrix();
		scenegraph->getCamera()->setViewMatrix(viewMatrix);

		angleDifference.set(0, 0);
	}
	else if (eyeMode) {
		scenegraph->getCamera()->setViewMatrix(Vector3(
			proximity*sin((float)(PI * 2) - viewAngle.x) * cos(viewAngle.y),
			proximity*sin(viewAngle.y),
			proximity*cos((float)(PI * 2) - viewAngle.x)*cos(viewAngle.y)),
			Vector3(0, 0, 0), Vector3(0, 1, 0));

	}
	else
	{
		scenegraph->getCamera()->setViewMatrix(Matrix::createTranslationMatrix(forwardAndbacward, upAndDown, -proximity)
			* Matrix::createRotationMatrix(viewAngle.y, viewAngle.x, 0.0f));
	}
}

void resetObj(int i)
{
	SceneNode* rootCopy = scenegraph->getRoot();
	SceneNode* mainNodeCopy = rootCopy->getChildAt(0);
	SceneNode* modelCopy = mainNodeCopy->getChildAt(0);
	SceneNode* obj = modelCopy->getChildById(i);

	if (obj != NULL && obj->pickable) {
		obj->resetMaterial();
	}
}

Material* pickedMaterial = new Material(Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1), 100);
void manipulateObj(int i, Vector3 pos)
{
	SceneNode* rootCopy = scenegraph->getRoot();
	SceneNode* mainNodeCopy = rootCopy->getChildAt(0);
	SceneNode* modelCopy = mainNodeCopy->getChildAt(0);
	SceneNode* obj = modelCopy->getChildById(i);

	if (obj != NULL && obj->pickable) {
		obj->setMaterial(pickedMaterial);
		obj->setPosition(pos.x, pos.y-1, pos.z);
		objPicked = i;
	}
	else { 
			objPicked = 0; 
		 }
	
}



void drawScene()
{
	scenegraph->draw();

	
	if (picked) {
		Vector3 newPos = Vector3(-rootPosition.x, -rootPosition.y - 1, -rootPosition.z);
		manipulateObj(objPicked, newPos);
	}
	else resetObj(objPicked);
	checkOpenGLError("ERROR: Could not draw scene.");
}


/////////////////////////////////////////////////////////////////////// CALLBACKs

void cleanup()
{
	destroyScene();
}

void display()
{
	++FrameCount;
	shaderProgram->clear();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	act((float)deltaTime / 1000); //miliseconds to seconds
	changeView();
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
	shaderProgram->reshape(Vector2((float)WinX, (float)WinY));
	scenegraph->getCamera()->setPerspectiveProjection(((float)PI / 2.0f), (float)WinX / WinY, 0.1f, 100);

}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void normalKeysDownCallback(unsigned char key, int xmouse, int ymouse) {


	switch (key) {
	case 'b':
		timeline->setIsReversed(!timeline->isReversed());
		if (!timeline->isRunning())
			timeline->start();
		break;
	
	case 'q':
		quaternianMode = !quaternianMode;
		break;
	case 'e':
		eyeMode = !eyeMode;
		break;
	}

	keyManager.normalKeyDownHandler(key, xmouse, ymouse);
}

void normalKeysUpCallback(unsigned char key, int xmouse, int ymouse) {
	keyManager.normalKeyUpHandler(key, xmouse, ymouse);
}

void specialKeysDownCallback(int key, int xmouse, int ymouse) {
	keyManager.specialKeyDownHandler(key, xmouse, ymouse);
}

void specialKeysUpCallback(int key, int xmouse, int ymouse) {
	keyManager.specialKeyUpHandler(key, xmouse, ymouse);
}

void getMouseClicks(int button, int state, int x, int y) {
	int px = WinX / 2;
	int py = WinY / 2;
	Matrix4 viewModel = scenegraph->getCamera()->getViewMatrix();
	Vector3 cameraPos = Vector3(viewModel[12], viewModel[13], viewModel[14]) + Vector3(0, 0, -0.5);

	if (button == 3 && state == GLUT_UP) { //wheel down
		proximity -= 0.1f;
	}
	else if (button == 4 && state == GLUT_UP) { //wheel up
		proximity += 0.1f;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		inMovement = true;
		mousePosition.set((float)x, (float)y);

		if (!picked) {
			shaderProgram->begin();

			glReadPixels(px, WinY - py - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			glReadPixels(px, WinY - py - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glReadPixels(px, WinY - py - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

			shaderProgram->end();

			id = index - 1;

			if (depth > 0.99)
				return;
			manipulateObj(id, Vector3(-rootPosition.x, -rootPosition.y - 1, -rootPosition.z));
		}
		else {
			manipulateObj(id, Vector3(-rootPosition.x, -rootPosition.y - 2, -rootPosition.z));
			if (gameManager.isGameOver()) {
				//std::cout << "OVER!" << std::endl;
				doorNode->setPickable(true);
				
			}
				
		}
		printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
			px, py, color[0], color[1], color[2], color[3], depth, index);
		picked = !picked;

	}


}



void getMouseMovement(int xm, int ym) {
	

	Vector2 newPosition((float)xm, (float)ym);
	Vector2 difference = newPosition - mousePosition;

	float resultX = (float)PI * difference.x / (WinX / 2);
	float resutlY = (float)PI * difference.y / (WinY / 2);

	viewAngle.x += resultX*2; viewAngle.y += resutlY*2;
	angleDifference.x += resultX*2; angleDifference.y += resutlY*2;

	mousePosition = newPosition;

	
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(normalKeysDownCallback);
	glutKeyboardUpFunc(normalKeysUpCallback);
	glutSpecialFunc(specialKeysDownCallback);
	glutSpecialUpFunc(specialKeysUpCallback);
	glutMouseFunc(getMouseClicks);
	glutPassiveMotionFunc(getMouseMovement);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createScene();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

//Hint that we want to work on the graphics card
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}