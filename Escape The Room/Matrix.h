#pragma once
#include <iostream>
#include "GL\glew.h"
#include "Vector.h"

class Matrix2;
class Matrix3;
class Matrix4;

class Matrix
{
public:
	GLfloat* getValues();
	static Matrix4 createScaleMatrix(float scaleX, float scaleY, float scaleZ);
	static Matrix4 createRotationMatrix(float rotationX, float rotationY, float rotationZ);
	static Matrix4 createRotationXMatrix(float rotationX);
	static Matrix4 createRotationYMatrix(float rotationY);
	static Matrix4 createRotationZMatrix(float rotationZ);
	static Matrix4 createTranslationMatrix(float translationX, float translationY, float translationZ);
protected:
	GLfloat values[16];
};

enum Matrix_Type { ZEROS_MATRIX, IDENTITY_MATRIX};

/*////////////////////////////////
//			MATRIX 4			//
////////////////////////////////*/



class Matrix4 : public Matrix
{
public:
	Matrix4();
	Matrix4(Matrix& matrix);
	Matrix4(Matrix_Type type);
	Matrix4(
		float c1r1, float c1r2, float c1r3, float c1r4,
		float c2r1, float c2r2, float c2r3, float c2r4,
		float c3r1, float c3r2, float c3r3, float c3r4,
		float c4r1, float c4r2, float c4r3, float c4r4);


	void set(Matrix& matrix);
	void set(Matrix_Type type);
	void set(
		float c1r1, float c1r2, float c1r3, float c1r4,
		float c2r1, float c2r2, float c2r3, float c2r4,
		float c3r1, float c3r2, float c3r3, float c3r4,
		float c4r1, float c4r2, float c4r3, float c4r4);

	//Extra matrix operations
	Matrix3 toMatrix3();
	//Matrix4 inverse();
	//float determinant();

	//Matrix access
	float operator[](const int position);

	//Unary operators
	//Matrix4 operator- ();
	//Matrix4 operator+ ();

	//Matrix with matrix operations
	void operator=(Matrix4& matrix);
	//bool operator!=(Matrix4& matrix);
	//bool operator==(Matrix4& matrix);
	//Matrix4 operator+(Matrix4& matrix);
	//Matrix4 operator-(Matrix4& matrix);
	Matrix4 operator*(Matrix4& matrix);
	//Matrix4 operator+=(Matrix4& matrix);
	//Matrix4 operator-=(Matrix4& matrix);
	Matrix4 operator*=(Matrix4& matrix);

	//Matrix with scalar operations
	//Matrix4 operator*(const float scalar);
	//Matrix4 operator/(const float scalar);
	//Matrix4 operator*=(const float scalar);
	//Matrix4 operator/=(const float scalar);

	//Matrix with vector operation
	Vector3 operator*(const Vector3& vector);

};

//Matrix4 operator*(const float scalar, Matrix4& matrix);

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix4& matrix);


/*////////////////////////////////
//			MATRIX 2			//
////////////////////////////////*/

class Matrix2 : public Matrix
{
public:
	Matrix2();
	Matrix2(Matrix_Type type);
	Matrix2(float c1r1, float c1r2, float c2r1, float c2r2);
	
	void set(Matrix2& matrix);
	void set(Matrix_Type type);
	void set(float c1r1, float c1r2, float c2r1, float c2r2);

	//Extra matrix operations
	Matrix2 transpose();
	Matrix2 inverse();
	float determinant();
	Matrix4 toMatrix4();
	
	//Matrix access
	float operator[](const int position);

	//Unary operators
	Matrix2 operator- ();
	Matrix2 operator+ ();

	//Matrix with matrix operations
	void operator=(Matrix2& matrix);
	bool operator!=(Matrix2& matrix);
	bool operator==(Matrix2& matrix);
	Matrix2 operator+(Matrix2& matrix);
	Matrix2 operator-(Matrix2& matrix);
	Matrix2 operator*(Matrix2& matrix);
	Matrix2 operator+=(Matrix2& matrix);
	Matrix2 operator-=(Matrix2& matrix);
	Matrix2 operator*=(Matrix2& matrix);

	//Matrix with scalar operations
	Matrix2 operator*(const float scalar);
	Matrix2 operator/(const float scalar);
	Matrix2 operator*=(const float scalar);
	Matrix2 operator/=(const float scalar);

	//Matrix with vector operation
	Vector2 operator*(const Vector2& vector);


private:
	const static int matrix2ToMatrixIndex[4];
};

Matrix2 operator*(const float scalar, Matrix2& matrix);

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix2& matrix);



/*////////////////////////////////
//			MATRIX 3			//
////////////////////////////////*/



class Matrix3 : public Matrix
{
public:
	Matrix3();
	Matrix3(Matrix_Type type);
	Matrix3(
		float c1r1, float c1r2, float c1r3,
		float c2r1, float c2r2, float c2r3,
		float c3r1, float c3r2, float c3r3);
	
	
	void set(Matrix3& matrix);
	void set(Matrix_Type type);
	void set(
		float c1r1, float c1r2, float c1r3,
		float c2r1, float c2r2, float c2r3,
		float c3r1, float c3r2, float c3r3);

	//Extra matrix operations
	Matrix3 transpose();
	Matrix3 inverse();
	float determinant();
	Matrix4 toMatrix4();

	//Matrix access
	float operator[](const int position);

	//Unary operators
	Matrix3 operator- ();
	Matrix3 operator+ ();

	//Matrix with matrix operations
	void operator=(Matrix3& matrix);
	bool operator!=(Matrix3& matrix);
	bool operator==(Matrix3& matrix);
	Matrix3 operator+(Matrix3& matrix);
	Matrix3 operator-(Matrix3& matrix);
	Matrix3 operator*(Matrix3& matrix);
	Matrix3 operator+=(Matrix3& matrix);
	Matrix3 operator-=(Matrix3& matrix);
	Matrix3 operator*=(Matrix3& matrix);

	//Matrix with scalar operations
	Matrix3 operator*(const float scalar);
	Matrix3 operator/(const float scalar);
	Matrix3 operator*=(const float scalar);
	Matrix3 operator/=(const float scalar);

	//Matrix with vector operation
	Vector3 operator*(const Vector3& vector);


private:
	const static int matrix3ToMatrixIndex[9];
};

Matrix3 operator*(const float scalar, Matrix3& matrix);

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix3& matrix);