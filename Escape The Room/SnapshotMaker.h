#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "SOIL\src\SOIL.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include <ctime>

/*////////////////////////////////
//	     SnapshotMaker 			//
////////////////////////////////*/

class SnapshotMaker {
private:
	int photosTaken = 0;
public:
	SnapshotMaker();
	void takePhoto(int winX, int winY);
	bool takeSnapshot(const char* filename, int x, int y);
};

