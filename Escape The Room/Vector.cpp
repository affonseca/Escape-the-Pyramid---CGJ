#include "Vector.h"

#define PI  3.14159265

const float qThreshold = (float)1.0e-5;

/*////////////////////////////////
//			VECTOR 2			//
////////////////////////////////*/

//VECTOR CLASS DEFINITIONS
Vector2::Vector2(float x, float y) {
	set(x, y);
}

Vector2::Vector2(const Vector2& vector) {
	set(vector);
}

Vector2::Vector2(void) {
	set(0, 0);
}

void Vector2::clean() {
	if (fabs(this->x) < qThreshold) this->x = 0.0f;
	if (fabs(this->y) < qThreshold) this->y = 0.0f;
}

void Vector2::operator=(const Vector2& vector)
{
	set(vector);
}

void Vector2::set(float x, float y) {
	this->x = x;
	this->y = y;
}

void Vector2::set(const Vector2& vector) {
	set(vector.x, vector.y);
}

//Extra operations
float Vector2::dot(const Vector2& vector) {
	return this->x * vector.x + this->y * vector.y;
}

float Vector2::getQuadrance() {
	return x*x + y*y;
}

float Vector2::getNorm() {
	return sqrt(getQuadrance());
}

Vector2 Vector2::getNormalized() {
	Vector2 out;
	out.x = x; out.y = y;
	float norm = getNorm();
	if (norm != 0)
		out /= norm;
	return out;
}

void Vector2::normalize() {
	set(getNormalized());
}

float Vector2::angleWithVector(Vector2& vector) {
	return (float)(acos(getNormalized().dot(vector.getNormalized())) * (180 / PI));
}

float Vector2::projectOn(Vector2& vector) {
	return dot(vector.getNormalized());
}

Vector2 Vector2::lerp(Vector2& vector, float percentage) {
	Vector2 out = vector - *this;
	out *= percentage; out += *this;
	return out;
}

//Unary operations
Vector2 Vector2::operator- () {
	return -1 * (*this);
}

Vector2 Vector2::operator+ () {
	return *this;
}

//Vector with vector operations
Vector2 Vector2::operator+=(const Vector2& vector) {
	this->x += vector.x; this->y += vector.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& vector) {
	this->x -= vector.x; this->y -= vector.y;
	return *this;
}


//Vector with scalar operations
Vector2 Vector2::operator*=(const float scalar) {
	this->x *= scalar; this->y *= scalar;
	return *this;
}

Vector2 Vector2::operator/=(const float scalar) {
	this->x /= scalar; this->y /= scalar;
	return *this;
}

//VECTOR OPERATIONS

//Vector printing
std::ostream& operator<<(std::ostream& out, const Vector2& vector) {
	return out << "( " << vector.x << ", " << vector.y << " )";
}

//Vector with vector operations
bool operator==(const Vector2& vector1, const Vector2& vector2) {
	return (
		fabs(vector1.x - vector2.x) < qThreshold &&
		fabs(vector1.y - vector2.y) < qThreshold);
}
bool operator!=(const Vector2& vector1, const Vector2& vector2) {
	return (
		fabs(vector1.x - vector2.x) > qThreshold ||
		fabs(vector1.y - vector2.y) > qThreshold);
}

Vector2 operator+(Vector2 vector1, const Vector2& vector2)
{
	return Vector2(vector1.x + vector2.x, vector1.y + vector2.y);
}

Vector2 operator-(Vector2 vector1, const Vector2& vector2)
{
	return Vector2(vector1.x - vector2.x, vector1.y - vector2.y);
}


//Vector with scalar operations
Vector2 operator*(Vector2 vector, const float scalar)
{
	return Vector2(vector.x * scalar, vector.y * scalar);
}

Vector2 operator*(const float scalar, Vector2 vector)
{
	return vector*scalar;
}

Vector2 operator/(Vector2 vector, const float scalar)
{
	return Vector2(vector.x / scalar, vector.y / scalar);
}

/*////////////////////////////////
//			VECTOR 3			//
////////////////////////////////*/

//VECTOR CLASS DEFINITIONS
Vector3::Vector3(float x, float y, float z) {
	set(x, y, z);
}

Vector3::Vector3(const Vector3& vector) {
	set(vector);
}

Vector3::Vector3(void) {
	set(0, 0, 0);
}

void Vector3::clean() {
	if (fabs(this->x) < qThreshold) this->x = 0.0f;
	if (fabs(this->y) < qThreshold) this->y = 0.0f;
	if (fabs(this->z) < qThreshold) this->z = 0.0f;
}

void Vector3::operator=(const Vector3& vector)
{
	set(vector);
}

void Vector3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::set(const Vector3& vector) {
	set(vector.x, vector.y, vector.z);
}

//Extra operations
float Vector3::dot(const Vector3& vector) {
	return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

float Vector3::getQuadrance() {
	return x*x + y*y + z*z;
}

float Vector3::getNorm() {
	return sqrt(getQuadrance());
}

Vector3 Vector3::getNormalized() {
	Vector3 out;
	out.x = x; out.y = y; out.z = z;
	float norm = getNorm();
	if (norm != 0)
		out /= norm;
	return out;
}

void Vector3::normalize() {
	set(getNormalized());
}

float Vector3::angleWithVector(Vector3& vector) {
	return (float)(acos(getNormalized().dot(vector.getNormalized())) * (180 / PI));
}

float Vector3::projectOn(Vector3& vector) {
	return dot(vector.getNormalized());
}

Vector3 Vector3::lerp(Vector3& vector, float percentage) {
	Vector3 out = vector - *this;
	out *= percentage; out += *this;
	return out;
}

//Unary operations
Vector3 Vector3::operator- () {
	return -1 * (*this);
}

Vector3 Vector3::operator+ () {
	return *this;
}

//Vector with vector operations
Vector3 Vector3::operator+=(const Vector3& vector) {
	this->x += vector.x; this->y += vector.y; this->z += vector.z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& vector) {
	this->x -= vector.x; this->y -= vector.y; this->z -= vector.z;
	return *this;
}

Vector3 Vector3::operator*=(const Vector3& vector) {
	Vector3 temp;
	temp.x = this->y * vector.z - this->z * vector.y;
	temp.y = this->z * vector.x - this->x * vector.z;
	temp.z = this->x * vector.y - this->y * vector.x;
	set(temp);
	return *this;
}


//Vector with scalar operations
Vector3 Vector3::operator*=(const float scalar) {
	this->x *= scalar; this->y *= scalar; this->z *= scalar;
	return *this;
}

Vector3 Vector3::operator/=(const float scalar) {
	this->x /= scalar; this->y /= scalar; this->z /= scalar;
	return *this;
}

//VECTOR OPERATIONS

//Vector printing
std::ostream& operator<<(std::ostream& out, const Vector3& vector) {
	return out << "( " << vector.x << ", " << vector.y << ", " << vector.z << " )";
}

//Vector with vector operations
bool operator==(const Vector3& vector1, const Vector3& vector2) {
	return (
		fabs(vector1.x - vector2.x) < qThreshold &&
		fabs(vector1.y - vector2.y) < qThreshold &&
		fabs(vector1.z - vector2.z) < qThreshold);
}
bool operator!=(const Vector3& vector1, const Vector3& vector2) {
	return (
		fabs(vector1.x - vector2.x) > qThreshold ||
		fabs(vector1.y - vector2.y) > qThreshold ||
		fabs(vector1.z - vector2.z) > qThreshold);
}

Vector3 operator+(const Vector3& vector1, const Vector3& vector2)
{
	return Vector3(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}

Vector3 operator-(const Vector3& vector1, const Vector3& vector2)
{
	return Vector3(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z);
}

Vector3 operator*(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 out;
	out.x = vector1.y * vector2.z - vector1.z * vector2.y;
	out.y = vector1.z * vector2.x - vector1.x * vector2.z;
	out.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return out;
}


//Vector with scalar operations
Vector3 operator*(const Vector3& vector, const float scalar)
{
	return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector3 operator*(const float scalar, const Vector3& vector)
{
	return vector*scalar;
}

Vector3 operator/(const Vector3& vector, const float scalar)
{
	return Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}