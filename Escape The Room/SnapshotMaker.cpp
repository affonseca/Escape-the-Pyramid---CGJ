#include "SnapshotMaker.h"

SnapshotMaker::SnapshotMaker()
{
}


bool SnapshotMaker::takeSnapshot(const char* filename, int w, int h)
{

	//This prevents the images getting padded 
	// when the width multiplied by 3 is not a multiple of 4
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w*h * 3;
	// First let's create our buffer, 3 channels per Pixel
	char* dataBuffer = (char*)malloc(nSize*sizeof(char));

	if (!dataBuffer) return false;

	glutSwapBuffers();
	// Let's fetch them from the backbuffer	
	glReadPixels((GLint)0, (GLint)0,
		(GLint)w, (GLint)h,
		GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);


	glutSwapBuffers();
	//Now the file creation
	FILE* filePtr;
	fopen_s(&filePtr ,filename, "wb");
	if (!filePtr) return false;


	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6] = { w % 256,w / 256,
		h % 256,h / 256,
		(unsigned char)24, (unsigned char)0 };

	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);

	delete[] dataBuffer;
	return true;
}

void SnapshotMaker::takePhoto(int winX, int winY)
{
	time_t t = time(0);   // get time now
	struct tm * now = new tm();
	localtime_s(now,&t);
	
	int day = now->tm_mday;
	int month = (now->tm_mon + 1);
	int year = (now->tm_year + 1900);

	int hour = (now->tm_hour);
	int min = (now->tm_min);
	int sec = (now->tm_sec);


	std::string score = "-";

	std::string imageName = std::string("../screenshots/screenshot ");
	imageName += std::to_string(day);
	imageName += score;
	imageName += std::to_string(month);
	imageName += score;
	imageName += std::to_string(year);
	imageName += score;
	imageName += std::to_string(hour);
	imageName += "h-";
	imageName += std::to_string(min);
	imageName += "m-";
	imageName += std::to_string(sec);
	imageName += "s";
	imageName += ".tga";
	std::cout << imageName << std::endl;
	this->takeSnapshot(imageName.c_str(), winX, winY);
	photosTaken++;
}
