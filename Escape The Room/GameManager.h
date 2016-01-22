#pragma once
#include <vector>
#include "Vector.h"
class GameManager {
private:
	int boundingBox00;
	int boundingBox01;
	std::vector<Vector3> keyPositions;
	float MAPXBOUNDINF;
	float MAPXBOUNDSUP;
	float MAPZBOUNDINF;
	float MAPZBOUNDSUP;

public:
	GameManager();
	bool isGameOver();
	void updatePositions(std::vector<Vector3> inputKeyPositions);
	bool checkPlayerPosition(Vector3 playerPos);

};