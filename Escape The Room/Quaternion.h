#pragma once

#include <iostream>
#include "GL\glew.h"
#include "Matrix.h"
#include "Vector.h"

class Quaternion{

public:
	GLfloat t, x, y, z;

	//Constructor
	Quaternion();
	Quaternion(GLfloat t, GLfloat x, GLfloat y, GLfloat z);
	Quaternion(const Quaternion& quaternion);
	Quaternion(float theta, Vector3 axis);

	//setters
	void set(GLfloat t, GLfloat x, GLfloat y, GLfloat z);
	void set(const Quaternion& quaternion);
	void set(float theta, Vector3 axis);

	//Back to angle and axis
	void toAngleAxis(float& theta, Vector3& axis);
	void clean();

	//Extra operations
	float getQuadrance();
	float getNorm();
	Quaternion getNormalized();
	void normalize();
	Quaternion conjugate();
	Quaternion inverse();
	Quaternion lerp(Quaternion& quaternion, float percentage);
	Quaternion slerp(Quaternion& quaternion, float percentage);
	Matrix4 toMatrix();

	//Unary operators
	Quaternion operator- ();
	Quaternion operator+ ();

	//Quaternion with quaternion operations
	void operator=(const Quaternion& quaternion);
	bool operator!=(const Quaternion& quaternion);
	bool operator==(const Quaternion& quaternion);
	Quaternion operator+(const Quaternion& quaternion);
	Quaternion operator-(const Quaternion& quaternion);
	Quaternion operator*(const Quaternion& quaternion);
	Quaternion operator+=(const Quaternion& quaternion);
	Quaternion operator-=(const Quaternion& quaternion);
	Quaternion operator*=(const Quaternion& quaternion);

	//Quaternion with scalar operations
	Quaternion operator*(const float scalar);
	Quaternion operator/(const float scalar);
	Quaternion operator*=(const float scalar);
	Quaternion operator/=(const float scalar);

	//print as angle axis
	void printAsAngleAxis();
	
private:
	const GLfloat qThreshold = (GLfloat)1.0e-5;
};


Quaternion operator*(const float scalar, Quaternion& quaternion);

//Matrix printing
std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion);