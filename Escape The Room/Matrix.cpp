#include "Matrix.h"

GLfloat* Matrix::getValues() {
	return values;
}

Matrix4 Matrix::createScaleMatrix(float scaleX, float scaleY, float scaleZ) {
	return Matrix4(
		scaleX,	0.0f,	0.0f,	0.0f,
		0.0f,	scaleY, 0.0f,	0.0f,
		0.0f,	0.0f,	scaleZ,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
		);
}

Matrix4 Matrix::createRotationXMatrix(float rotationX) {
	return Matrix4(
		1.0f,	0.0f,				0.0f,				0.0f,
		0.0f,	cosf(rotationX),	sinf(rotationX),	0.0f,
		0.0f,	-sinf(rotationX),	cosf(rotationX),	0.0f,
		0.0f,	0.0f,				0.0f,				1.0f
		);
}

Matrix4 Matrix::createRotationYMatrix(float rotationY) {
	return Matrix4(
		cosf(rotationY),	0.0f,	-sinf(rotationY),	0.0f,
		0.0f,				1.0f,	0.0f,				0.0f,
		sinf(rotationY),	0.0f,	cosf(rotationY),	0.0f,
		0.0f,				0.0f,	0.0f,				1.0f
		);
}

Matrix4 Matrix::createRotationZMatrix(float rotationZ) {
	return Matrix4(
		cosf(rotationZ),	sinf(rotationZ),	0.0f,	0.0f,
		-sinf(rotationZ),	cosf(rotationZ),	0.0f,	0.0f,
		0.0f,				0.0f,				1.0f,	0.0f,
		0.0f,				0.0f,				0.0f,	1.0f
		);
}

Matrix4 Matrix::createRotationMatrix(float rotationX, float rotationY, float rotationZ) {
	return createRotationXMatrix(rotationX) * createRotationYMatrix(rotationY) * createRotationZMatrix(rotationZ);
}


Matrix4 Matrix::createTranslationMatrix(float translationX, float translationY, float translationZ) {
	return Matrix4(
		1.0f,			0.0f,			0.0f,			0.0f,
		0.0f,			1.0f,			0.0f,			0.0f,
		0.0f,			0.0f,			1.0f,			0.0f,
		translationX,	translationY,	translationZ,	1.0f
		);
}

/*////////////////////////////////
//			MATRIX 2			//
////////////////////////////////*/

const int Matrix2::matrix2ToMatrixIndex[] = { 0, 1, 4, 5 };

Matrix2::Matrix2() {
	std::fill_n(values, 16, (float)0);
	values[10] = 1; values[15] = 1;
}

Matrix2::Matrix2(Matrix_Type type) {
	std::fill_n(values, 16, (float)0);
	values[10] = 1; values[15] = 1;
	set(type);
}

Matrix2::Matrix2(float c1r1, float c1r2, float c2r1, float c2r2) {
	std::fill_n(values, 16, (float)0);
	values[10] = 1; values[15] = 1;
	set(c1r1, c1r2, c2r1, c2r2);
}

void Matrix2::set(Matrix2& matrix) {
	set(matrix[0], matrix[1], matrix[2], matrix[3]);
}

void Matrix2::set(Matrix_Type type) {
	switch (type)
	{
	case ZEROS_MATRIX:
		set(0, 0, 0, 0);
		break;
	case IDENTITY_MATRIX:
		set(1, 0, 0, 1);
		break;
	default:
		break;
	}
}

void Matrix2::set(float c1r1, float c1r2, float c2r1, float c2r2) {
	values[matrix2ToMatrixIndex[0]] = c1r1;
	values[matrix2ToMatrixIndex[1]] = c1r2;
	values[matrix2ToMatrixIndex[2]] = c2r1;
	values[matrix2ToMatrixIndex[3]] = c2r2;
}

//Extra matrix operations
Matrix2 Matrix2::transpose() {
	return Matrix2((*this)[0], (*this)[2], (*this)[1], (*this)[3]);
}

Matrix2 Matrix2::inverse() {
	Matrix2 adj = Matrix2((*this)[3], -(*this)[1], -(*this)[2], (*this)[0]);
	return adj / determinant();
}

float Matrix2::determinant() {
	return (*this)[0] * (*this)[3] - (*this)[2] * (*this)[1];
}

Matrix4 Matrix2::toMatrix4() {
	return Matrix4(*this);
}

//Matrix access
float Matrix2::operator[](const int position) {
	return values[matrix2ToMatrixIndex[position]];
}

//Unary operators
Matrix2 Matrix2::operator- () {
	return -1 * (*this);
}

Matrix2 Matrix2::operator+ () {
	return Matrix2(*this);
}

//Matrix with matrix operations
void Matrix2::operator=(Matrix2& matrix) {
	set(matrix);
}

bool Matrix2::operator!=(Matrix2& matrix) {
	return (matrix[0] != (*this)[0] || matrix[1] != (*this)[1] ||
		matrix[2] != (*this)[2] || matrix[3] != (*this)[3]);
}

bool Matrix2::operator==(Matrix2& matrix) {
	return (matrix[0] == (*this)[0] && matrix[1] == (*this)[1] &&
		matrix[2] == (*this)[2] && matrix[3] == (*this)[3]);
}

Matrix2 Matrix2::operator+(Matrix2& matrix) {
	return Matrix2(
		(*this)[0] + matrix[0],
		(*this)[1] + matrix[1],
		(*this)[2] + matrix[2],
		(*this)[3] + matrix[3]);
}

Matrix2 Matrix2::operator-(Matrix2& matrix) {
	return Matrix2(
		(*this)[0] - matrix[0],
		(*this)[1] - matrix[1],
		(*this)[2] - matrix[2],
		(*this)[3] - matrix[3]);
}

Matrix2 Matrix2::operator*(Matrix2& matrix) {
	return Matrix2(
		(*this)[0] * matrix[0] + (*this)[2] * matrix[1],
		(*this)[1] * matrix[0] + (*this)[3] * matrix[1],
		(*this)[0] * matrix[2] + (*this)[2] * matrix[3],
		(*this)[1] * matrix[2] + (*this)[3] * matrix[3]);
}

Matrix2 Matrix2::operator+=(Matrix2& matrix) {
	Matrix2 aux = Matrix2((*this) + matrix);
	set(aux);
	return (*this);
}

Matrix2 Matrix2::operator-=(Matrix2& matrix) {
	Matrix2 aux = Matrix2((*this) - matrix);
	set(aux);
	return (*this);
}

Matrix2 Matrix2::operator*=(Matrix2& matrix) {
	Matrix2 aux = Matrix2((*this) * matrix);
	set(aux);
	return (*this);
}


//Matrix with scalar operations
Matrix2 Matrix2::operator*(const float scalar) {
	return Matrix2(
		scalar*(*this)[0], scalar*(*this)[1],
		scalar*(*this)[2], scalar*(*this)[3]);
}

Matrix2 Matrix2::operator/(const float scalar) {
	return Matrix2(
		(*this)[0] / scalar, (*this)[1] / scalar,
		(*this)[2] / scalar, (*this)[3] / scalar);
}

Matrix2 Matrix2::operator*=(const float scalar) {
	set(scalar*(*this)[0], scalar*(*this)[1],
		scalar*(*this)[2], scalar*(*this)[3]);
	return (*this);
}

Matrix2 Matrix2::operator/=(const float scalar) {
	set(
		(*this)[0] / scalar, (*this)[1] / scalar,
		(*this)[2] / scalar, (*this)[3] / scalar);
	return (*this);
}

Vector2 Matrix2::operator*(const Vector2& vector) {
	return Vector2(
		(*this)[0] * vector.x + (*this)[2] * vector.y,
		(*this)[1] * vector.x + (*this)[3] * vector.y);
}

Matrix2 operator*(const float scalar, Matrix2& matrix) {
	return matrix * scalar;
}

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix2& matrix) {
	return out << "[ " << matrix[0] << "  " << matrix[2] << " ]\n"
		<< "[ " << matrix[1] << "  " << matrix[3] << " ]\n";
}

/*////////////////////////////////
//			MATRIX 3			//
////////////////////////////////*/

const int Matrix3::matrix3ToMatrixIndex[] = { 0, 1, 2, 4, 5, 6, 8, 9, 10 };

Matrix3::Matrix3() {
	std::fill_n(values, 16, (float)0);
	values[15] = 1;
}

Matrix3::Matrix3(Matrix_Type type) {
	std::fill_n(values, 16, (float)0);
	values[15] = 1;
	set(type);
}

Matrix3::Matrix3(
	float c1r1, float c1r2, float c1r3,
	float c2r1, float c2r2, float c2r3,
	float c3r1, float c3r2, float c3r3) {
	std::fill_n(values, 16, (float)0);
	values[15] = 1;
	set(c1r1, c1r2, c1r3, c2r1, c2r2, c2r3, c3r1, c3r2, c3r3);
}

void Matrix3::set(Matrix3& matrix) {
	set(
		matrix[0], matrix[1], matrix[2],
		matrix[3], matrix[4], matrix[5],
		matrix[6], matrix[7], matrix[8]);
}

void Matrix3::set(Matrix_Type type) {
	switch (type)
	{
	case ZEROS_MATRIX:
		set(0, 0, 0, 0, 0, 0, 0, 0, 0);
		break;
	case IDENTITY_MATRIX:
		set(1, 0, 0, 0, 1, 0, 0, 0, 1);
		break;
	default:
		break;
	}
}

void Matrix3::set(
	float c1r1, float c1r2, float c1r3,
	float c2r1, float c2r2, float c2r3,
	float c3r1, float c3r2, float c3r3) {
	values[matrix3ToMatrixIndex[0]] = c1r1;
	values[matrix3ToMatrixIndex[1]] = c1r2;
	values[matrix3ToMatrixIndex[2]] = c1r3;
	values[matrix3ToMatrixIndex[3]] = c2r1;
	values[matrix3ToMatrixIndex[4]] = c2r2;
	values[matrix3ToMatrixIndex[5]] = c2r3;
	values[matrix3ToMatrixIndex[6]] = c3r1;
	values[matrix3ToMatrixIndex[7]] = c3r2;
	values[matrix3ToMatrixIndex[8]] = c3r3;
}

//Extra matrix operations
Matrix3 Matrix3::transpose() {
	return Matrix3(
		(*this)[0], (*this)[3], (*this)[6],
		(*this)[1], (*this)[4], (*this)[7],
		(*this)[2], (*this)[5], (*this)[8]);
}

Matrix3 Matrix3::inverse() {
	float m11 = Matrix2((*this)[4], (*this)[5], (*this)[7], (*this)[8]).determinant();
	float m12 = Matrix2((*this)[3], (*this)[5], (*this)[6], (*this)[8]).determinant();
	float m13 = Matrix2((*this)[3], (*this)[4], (*this)[6], (*this)[7]).determinant();
	float m21 = Matrix2((*this)[1], (*this)[2], (*this)[7], (*this)[8]).determinant();
	float m22 = Matrix2((*this)[0], (*this)[2], (*this)[6], (*this)[8]).determinant();
	float m23 = Matrix2((*this)[0], (*this)[1], (*this)[6], (*this)[7]).determinant();
	float m31 = Matrix2((*this)[1], (*this)[2], (*this)[4], (*this)[5]).determinant();
	float m32 = Matrix2((*this)[0], (*this)[2], (*this)[3], (*this)[5]).determinant();
	float m33 = Matrix2((*this)[0], (*this)[1], (*this)[3], (*this)[4]).determinant();

	Matrix3 adj = Matrix3(m11, -m21, m31, -m12, m22, -m32, m13, -m23, m33);
	return adj / determinant();
}

float Matrix3::determinant() {
	float m1 = Matrix2((*this)[4], (*this)[5], (*this)[7], (*this)[8]).determinant();
	float m2 = Matrix2((*this)[3], (*this)[5], (*this)[6], (*this)[8]).determinant();
	float m3 = Matrix2((*this)[3], (*this)[4], (*this)[6], (*this)[7]).determinant();

	return (*this)[0] * m1 - (*this)[1] * m2 + (*this)[2] * m3;
}


Matrix4 Matrix3::toMatrix4() {
	return Matrix4(*this);
}

//Matrix access
float Matrix3::operator[](const int position) {
	return values[matrix3ToMatrixIndex[position]];
}

//Unary operators
Matrix3 Matrix3::operator- () {
	return -1 * (*this);
}

Matrix3 Matrix3::operator+ () {
	return Matrix3(*this);
}

//Matrix with matrix operations
void Matrix3::operator=(Matrix3& matrix) {
	set(matrix);
}

bool Matrix3::operator!=(Matrix3& matrix) {
	return (
		matrix[0] != (*this)[0] || matrix[1] != (*this)[1] || matrix[2] != (*this)[2] ||
		matrix[3] != (*this)[3] || matrix[4] != (*this)[4] || matrix[5] != (*this)[5] ||
		matrix[6] != (*this)[6] || matrix[7] != (*this)[7] || matrix[8] != (*this)[8]);
}

bool Matrix3::operator==(Matrix3& matrix) {
	return (
		matrix[0] == (*this)[0] && matrix[1] == (*this)[1] && matrix[2] && (*this)[2] &&
		matrix[3] == (*this)[3] && matrix[4] == (*this)[4] && matrix[5] && (*this)[5] &&
		matrix[6] == (*this)[6] && matrix[7] == (*this)[7] && matrix[8] && (*this)[8]);
}

Matrix3 Matrix3::operator+(Matrix3& matrix) {
	return Matrix3(
		(*this)[0] + matrix[0], (*this)[1] + matrix[1], (*this)[2] + matrix[2],
		(*this)[3] + matrix[3], (*this)[4] + matrix[4], (*this)[5] + matrix[5],
		(*this)[6] + matrix[6], (*this)[7] + matrix[7], (*this)[8] + matrix[8]);
}

Matrix3 Matrix3::operator-(Matrix3& matrix) {
	return Matrix3(
		(*this)[0] - matrix[0], (*this)[1] - matrix[1], (*this)[2] - matrix[2],
		(*this)[3] - matrix[3], (*this)[4] - matrix[4], (*this)[5] - matrix[5],
		(*this)[6] - matrix[6], (*this)[7] - matrix[7], (*this)[8] - matrix[8]);
}

Matrix3 Matrix3::operator*(Matrix3& matrix) {
	return Matrix3(
		(*this)[0] * matrix[0] + (*this)[3] * matrix[1] + (*this)[6] * matrix[2],
		(*this)[1] * matrix[0] + (*this)[4] * matrix[1] + (*this)[7] * matrix[2],
		(*this)[2] * matrix[0] + (*this)[5] * matrix[1] + (*this)[8] * matrix[2],
		(*this)[0] * matrix[3] + (*this)[3] * matrix[4] + (*this)[6] * matrix[5],
		(*this)[1] * matrix[3] + (*this)[4] * matrix[4] + (*this)[7] * matrix[5],
		(*this)[2] * matrix[3] + (*this)[5] * matrix[4] + (*this)[8] * matrix[5],
		(*this)[0] * matrix[6] + (*this)[3] * matrix[7] + (*this)[6] * matrix[8],
		(*this)[1] * matrix[6] + (*this)[4] * matrix[7] + (*this)[7] * matrix[8],
		(*this)[2] * matrix[6] + (*this)[5] * matrix[7] + (*this)[8] * matrix[8]);
}

Matrix3 Matrix3::operator+=(Matrix3& matrix) {
	Matrix3 aux = Matrix3((*this) + matrix);
	set(aux);
	return (*this);
}

Matrix3 Matrix3::operator-=(Matrix3& matrix) {
	Matrix3 aux = Matrix3((*this) - matrix);
	set(aux);
	return (*this);
}

Matrix3 Matrix3::operator*=(Matrix3& matrix) {
	Matrix3 aux = Matrix3((*this) * matrix);
	set(aux);
	return (*this);
}


//Matrix with scalar operations
Matrix3 Matrix3::operator*(const float scalar) {
	return Matrix3(
		scalar*(*this)[0], scalar*(*this)[1], scalar*(*this)[2],
		scalar*(*this)[3], scalar*(*this)[4], scalar*(*this)[5],
		scalar*(*this)[6], scalar*(*this)[7], scalar*(*this)[8]);
}

Matrix3 Matrix3::operator/(const float scalar) {
	return Matrix3(
		(*this)[0] / scalar, (*this)[1] / scalar, (*this)[2] / scalar,
		(*this)[3] / scalar, (*this)[4] / scalar, (*this)[5] / scalar,
		(*this)[6] / scalar, (*this)[7] / scalar, (*this)[8] / scalar);
}

Matrix3 Matrix3::operator*=(const float scalar) {
	set(scalar*(*this)[0], scalar*(*this)[1], scalar*(*this)[2],
		scalar*(*this)[3], scalar*(*this)[4], scalar*(*this)[5],
		scalar*(*this)[6], scalar*(*this)[7], scalar*(*this)[8]);
	return (*this);
}

Matrix3 Matrix3::operator/=(const float scalar) {
	set(
		(*this)[0] / scalar, (*this)[1] / scalar, (*this)[2] / scalar,
		(*this)[3] / scalar, (*this)[4] / scalar, (*this)[5] / scalar,
		(*this)[6] / scalar, (*this)[7] / scalar, (*this)[8] / scalar);
	return (*this);
}

Vector3 Matrix3::operator*(const Vector3& vector) {
	return Vector3(
		(*this)[0] * vector.x + (*this)[3] * vector.y + (*this)[6] * vector.z,
		(*this)[1] * vector.x + (*this)[4] * vector.y + (*this)[7] * vector.z,
		(*this)[2] * vector.x + (*this)[5] * vector.y + (*this)[8] * vector.z);
}

Matrix3 operator*(const float scalar, Matrix3& matrix) {
	return matrix * scalar;
}

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix3& matrix) {
	return out
		<< "[ " << matrix[0] << "  " << matrix[3] << "  " << matrix[6] << " ]\n"
		<< "[ " << matrix[1] << "  " << matrix[4] << "  " << matrix[7] << " ]\n"
		<< "[ " << matrix[2] << "  " << matrix[5] << "  " << matrix[8] << " ]\n";
}


/*////////////////////////////////
//			MATRIX 4			//
////////////////////////////////*/

Matrix4::Matrix4() {
	std::fill_n(values, 16, (float)0);
}

Matrix4::Matrix4(Matrix& matrix) {
	std::fill_n(values, 16, (float)0);
	set(matrix);
}

Matrix4::Matrix4(Matrix_Type type) {
	std::fill_n(values, 16, (float)0);
	set(type);
}

Matrix4::Matrix4(
	float c1r1, float c1r2, float c1r3, float c1r4,
	float c2r1, float c2r2, float c2r3, float c2r4,
	float c3r1, float c3r2, float c3r3, float c3r4,
	float c4r1, float c4r2, float c4r3, float c4r4) {
	std::fill_n(values, 16, (float)0);
	set(c1r1, c1r2, c1r3, c1r4,
		c2r1, c2r2, c2r3, c2r4,
		c3r1, c3r2, c3r3, c3r4,
		c4r1, c4r2, c4r3, c4r4);
}

void Matrix4::set(Matrix& matrix) {
	set(
		matrix.getValues()[0], matrix.getValues()[1], matrix.getValues()[2], matrix.getValues()[3],
		matrix.getValues()[4], matrix.getValues()[5], matrix.getValues()[6], matrix.getValues()[7],
		matrix.getValues()[8], matrix.getValues()[9], matrix.getValues()[10], matrix.getValues()[11],
		matrix.getValues()[12], matrix.getValues()[13], matrix.getValues()[14], matrix.getValues()[15]);
}

void Matrix4::set(Matrix_Type type) {
	switch (type)
	{
	case ZEROS_MATRIX:
		set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		break;
	case IDENTITY_MATRIX:
		set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		break;
	default:
		break;
	}
}

void Matrix4::set(
	float c1r1, float c1r2, float c1r3, float c1r4,
	float c2r1, float c2r2, float c2r3, float c2r4,
	float c3r1, float c3r2, float c3r3, float c3r4,
	float c4r1, float c4r2, float c4r3, float c4r4) {
	values[0] = c1r1;
	values[1] = c1r2;
	values[2] = c1r3;
	values[3] = c1r4;
	values[4] = c2r1;
	values[5] = c2r2;
	values[6] = c2r3;
	values[7] = c2r4;
	values[8] = c3r1;
	values[9] = c3r2;
	values[10] = c3r3;
	values[11] = c3r4;
	values[12] = c4r1;
	values[13] = c4r2;
	values[14] = c4r3;
	values[15] = c4r4;
}

//Extra Matrixoperations
Matrix3 Matrix4::toMatrix3() {
	return Matrix3(
		(*this)[0], (*this)[1], (*this)[2],
		(*this)[4], (*this)[5], (*this)[6],
		(*this)[8], (*this)[9], (*this)[10]
		);
}

//Matrix access
float Matrix4::operator[](const int position) {
	return values[position];
}

//Matrix with matrix operations
void Matrix4::operator=(Matrix4& matrix) {
	set(matrix);
}

Matrix4 Matrix4::operator*(Matrix4& matrix) {
	return Matrix4(
		(*this)[0] * matrix[0] + (*this)[4] * matrix[1] + (*this)[8] * matrix[2] + (*this)[12] * matrix[3],
		(*this)[1] * matrix[0] + (*this)[5] * matrix[1] + (*this)[9] * matrix[2] + (*this)[13] * matrix[3],
		(*this)[2] * matrix[0] + (*this)[6] * matrix[1] + (*this)[10] * matrix[2] + (*this)[14] * matrix[3],
		(*this)[3] * matrix[0] + (*this)[7] * matrix[1] + (*this)[11] * matrix[2] + (*this)[15] * matrix[3],
		(*this)[0] * matrix[4] + (*this)[4] * matrix[5] + (*this)[8] * matrix[6] + (*this)[12] * matrix[7],
		(*this)[1] * matrix[4] + (*this)[5] * matrix[5] + (*this)[9] * matrix[6] + (*this)[13] * matrix[7],
		(*this)[2] * matrix[4] + (*this)[6] * matrix[5] + (*this)[10] * matrix[6] + (*this)[14] * matrix[7],
		(*this)[3] * matrix[4] + (*this)[7] * matrix[5] + (*this)[11] * matrix[6] + (*this)[15] * matrix[7],
		(*this)[0] * matrix[8] + (*this)[4] * matrix[9] + (*this)[8] * matrix[10] + (*this)[12] * matrix[11],
		(*this)[1] * matrix[8] + (*this)[5] * matrix[9] + (*this)[9] * matrix[10] + (*this)[13] * matrix[11],
		(*this)[2] * matrix[8] + (*this)[6] * matrix[9] + (*this)[10] * matrix[10] + (*this)[14] * matrix[11],
		(*this)[3] * matrix[8] + (*this)[7] * matrix[9] + (*this)[11] * matrix[10] + (*this)[15] * matrix[11],
		(*this)[0] * matrix[12] + (*this)[4] * matrix[13] + (*this)[8] * matrix[14] + (*this)[12] * matrix[15],
		(*this)[1] * matrix[12] + (*this)[5] * matrix[13] + (*this)[9] * matrix[14] + (*this)[13] * matrix[15],
		(*this)[2] * matrix[12] + (*this)[6] * matrix[13] + (*this)[10] * matrix[14] + (*this)[14] * matrix[15],
		(*this)[3] * matrix[12] + (*this)[7] * matrix[13] + (*this)[11] * matrix[14] + (*this)[15] * matrix[15]);
}

Matrix4 Matrix4::operator*=(Matrix4& matrix) {
	Matrix4 aux = Matrix4((*this) * matrix);
	set(aux);
	return (*this);
}

Vector3 Matrix4::operator*(const Vector3& vector) {
	Vector3 out = this->toMatrix3()*vector;
	out.x += (*this)[12]; //w == 1
	out.y += (*this)[13]; //w == 1
	out.z += (*this)[14]; //w == 1

	GLfloat w = (*this)[3] * vector.x + (*this)[7] * vector.y + (*this)[11] * vector.z + (*this)[15];
	out /= w;
	return out;
}

//Matrix printing
std::ostream& operator<<(std::ostream& out, Matrix4& matrix) {
	return out
		<< "[ " << matrix[0] << "  " << matrix[4] << "  " << matrix[8] << "  " << matrix[12] << " ]\n"
		<< "[ " << matrix[1] << "  " << matrix[5] << "  " << matrix[9] << "  " << matrix[13] << " ]\n"
		<< "[ " << matrix[2] << "  " << matrix[6] << "  " << matrix[10] << "  " << matrix[14] << " ]\n"
		<< "[ " << matrix[3] << "  " << matrix[7] << "  " << matrix[11] << "  " << matrix[15] << " ]\n";
}