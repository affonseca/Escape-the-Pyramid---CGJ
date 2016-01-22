#include "KeyManager.h"

KeyManager::KeyManager() {
	for (int i = 0; i < 256; i++) {
		keys[i] = false;
		specialKeys[i] = false;
	}
}

void KeyManager::normalKeyUpHandler(unsigned char key, int xmouse, int ymouse) {
	keys[key] = false;
}

void KeyManager::normalKeyDownHandler(unsigned char key, int xmouse, int ymouse) {
	keys[key] = true;
}

void KeyManager::specialKeyUpHandler(int key, int xmouse, int ymouse) {
	specialKeys[key] = false;
}

void KeyManager::specialKeyDownHandler(int key, int xmouse, int ymouse) {
	specialKeys[key] = true;
}

bool KeyManager::isNormalPressed(int key) {
	return keys[key];
}

bool KeyManager::isSpecialPressed(int key) {
	return specialKeys[key];
}