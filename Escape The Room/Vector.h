#pragma once
#include <iostream>
#include <math.h>

/*////////////////////////////////
//			VECTOR 2			//
////////////////////////////////*/

class Vector2 {
public:
	float x, y;
	Vector2(float x, float y);
	Vector2(const Vector2& vector);
	Vector2(void);

	void clean();

	void operator=(const Vector2& vector);
	void set(float x, float y);
	void set(const Vector2& vector);

	//Extra operations
	float getQuadrance();
	float getNorm();
	Vector2 getNormalized();
	void normalize();
	float angleWithVector(Vector2& vector);
	float projectOn(Vector2& vector);
	Vector2 lerp(Vector2& vector, float percentage);

	//Unary operators
	Vector2 operator- ();
	Vector2 operator+ ();

	//Vector with vector operations
	Vector2 operator+=(const Vector2& vector);
	Vector2 operator-=(const Vector2& vector);

	//Vector with scalar operations
	Vector2 operator*=(const float scalar);
	Vector2 operator/=(const float scalar);

private:
	float dot(const Vector2& vector);
};


//Vector printing
std::ostream& operator<<(std::ostream& out, const Vector2& vector);

//Vector with vector operations
bool operator==(const Vector2& vector1, const Vector2& vector2);
bool operator!=(const Vector2& vector1, const Vector2& vector2);
Vector2 operator+(Vector2 vector1, const Vector2& vector2);
Vector2 operator-(Vector2 vector1, const Vector2& vector2);

//Vector with scalar operations
Vector2 operator*(Vector2 vector, const float scalar);
Vector2 operator*(const float scalar, Vector2 vector);
Vector2 operator/(Vector2 vector, const float scalar);



/*////////////////////////////////
//			VECTOR 3			//
////////////////////////////////*/


class Vector3 {
public:
	float x, y, z;
	Vector3(float x, float y, float z);
	Vector3(const Vector3& vector);
	Vector3(void);

	void clean();

	void operator=(const Vector3& vector);
	void set(float x, float y, float z);
	void set(const Vector3& vector);

	//Extra operations
	float dot(const Vector3& vector);
	float getQuadrance();
	float getNorm();
	Vector3 getNormalized();
	void normalize();
	float angleWithVector(Vector3& vector);
	float projectOn(Vector3& vector);
	Vector3 lerp(Vector3& vector, float percentage);

	//Unary operators
	Vector3 operator- ();
	Vector3 operator+ ();

	//Vector with vector operations
	Vector3 operator+=(const Vector3& vector);
	Vector3 operator-=(const Vector3& vector);
	Vector3 operator*=(const Vector3& vector);

	//Vector with scalar operations
	Vector3 operator*=(const float scalar);
	Vector3 operator/=(const float scalar);

};


//Vector printing
std::ostream& operator<<(std::ostream& out, const Vector3& vector);

//Vector with vector operations
bool operator==(const Vector3& vector1, const Vector3& vector2);
bool operator!=(const Vector3& vector1, const Vector3& vector2);
Vector3 operator+(const Vector3&  vector1, const Vector3& vector2);
Vector3 operator-(const Vector3&  vector1, const Vector3& vector2);
Vector3 operator*(const Vector3&  vector1, const Vector3& vector2);

//Vector with scalar operations
Vector3 operator*(const Vector3& vector, const float scalar);
Vector3 operator*(const float scalar, const Vector3& vector);
Vector3 operator/(const Vector3& vector, const float scalar);