#pragma once

class KeyManager
{
public:
	KeyManager();
	void normalKeyUpHandler(unsigned char key, int xmouse, int ymouse);
	void normalKeyDownHandler(unsigned char key, int xmouse, int ymouse);
	void specialKeyUpHandler(int key, int xmouse, int ymouse);
	void specialKeyDownHandler(int key, int xmouse, int ymouse);
	bool isNormalPressed(int key);
	bool isSpecialPressed(int key);

private:
	bool keys[256];
	bool specialKeys[256];
};
