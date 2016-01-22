#include "GameManager.h"
#define PI		3.14159265
GameManager::GameManager()
{
	this->boundingBox00 = -1;
	this->boundingBox01 = 1;

	this->MAPXBOUNDINF=-5;
	this->MAPXBOUNDSUP=40;
	this->MAPZBOUNDINF=-40;
	this->MAPZBOUNDSUP=35;
}

bool GameManager::isGameOver()
{
	std::cout << this->keyPositions[0] << std::endl;
	std::cout << this->keyPositions[1] << std::endl;
	std::cout << this->keyPositions[2] << std::endl;
	std::cout << this->keyPositions[3] << std::endl;
	std::cout << this->boundingBox00 << std::endl;
	std::cout << this->boundingBox01 << std::endl;
	std::cout << (keyPositions[0].x > boundingBox00 && keyPositions[0].x < boundingBox01 || keyPositions[0].z > boundingBox00 && keyPositions[0].z < boundingBox01) << std::endl;
	std::cout << (keyPositions[1].x > boundingBox00 && keyPositions[1].x < boundingBox01 || keyPositions[1].z > boundingBox00 && keyPositions[1].z < boundingBox01) << std::endl;
	std::cout << (keyPositions[2].x > boundingBox00 && keyPositions[2].x < boundingBox01 || keyPositions[2].z > boundingBox00 && keyPositions[2].z < boundingBox01) << std::endl;
	std::cout << (keyPositions[3].x > boundingBox00 && keyPositions[3].x < boundingBox01 || keyPositions[3].z > boundingBox00 && keyPositions[3].z < boundingBox01) << std::endl;

	for (int i = 0; i < this->keyPositions.size(); i++) {
		
		if (!(keyPositions[i].x > boundingBox00 && keyPositions[i].x < boundingBox01 || keyPositions[i].z > boundingBox00 && keyPositions[i].z < boundingBox01)) {
			return false;
		}
	}
	return true;
}

void GameManager::updatePositions(std::vector<Vector3> inputKeyPositions)
{
	this->keyPositions = inputKeyPositions;
}

bool GameManager::checkPlayerPosition( Vector3 playerPos)
{
	std::cout << playerPos << std::endl;
	
	//if (viewAngle < 0) viewAngle = -viewAngle;
	if (playerPos.x<MAPXBOUNDSUP && playerPos.x>MAPXBOUNDINF
		&& playerPos.z<MAPZBOUNDSUP && playerPos.z>MAPZBOUNDINF) {
		
		return true;
	}
	return false;
}
