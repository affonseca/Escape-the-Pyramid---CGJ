#include "Quaternion.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913


//Constructor
Quaternion::Quaternion() {}

Quaternion::Quaternion(GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
	set(t, x, y, z);
}

Quaternion::Quaternion(const Quaternion& quaternion) {
	set(quaternion);
}

Quaternion::Quaternion(float theta, Vector3 axis) {
	set(theta, axis);
}

void Quaternion::set(GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
	this->t = t;
	this->x = x;
	this->y = y;
	this->z = z;
}

void Quaternion::set(const Quaternion& quaternion) {
	set(quaternion.t, quaternion.x, quaternion.y, quaternion.z);
}

void Quaternion::set(float theta, Vector3 axis) {
	Vector3 axisN = axis.getNormalized();

	float a = theta;
	float s = sin(a / 2.0f);

	set(cos(a / 2.0f), axisN.x * s, axisN.y * s, axisN.z * s);

	this->clean();
	this->normalize();
}

//Back to angle and axis
void Quaternion::toAngleAxis(float& theta, Vector3& axis) {
	Quaternion normalized = this->getNormalized();

	theta = 2.0f * acos(normalized.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - normalized.t*normalized.t);
	if (s < qThreshold) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
	}
	else {
		axis.x = normalized.x / s;
		axis.y = normalized.y / s;
		axis.z = normalized.z / s;
	}
}


void Quaternion::clean() {
	if (fabs(this->t) < qThreshold) this->t = 0.0f;
	if (fabs(this->x) < qThreshold) this->x = 0.0f;
	if (fabs(this->y) < qThreshold) this->y = 0.0f;
	if (fabs(this->z) < qThreshold) this->z = 0.0f;
}

//Extra operations
float Quaternion::getQuadrance() {
	return this->t*this->t + this->x*this->x + this->y*this->y + this->z*this->z;
}

float Quaternion::getNorm() {
	return sqrt(getQuadrance());
}


Quaternion Quaternion::getNormalized() {
	float norm = getNorm();
	return (norm == 0) ? Quaternion() : (*this) / norm;
}


void Quaternion::normalize() {
	set(getNormalized());
}


Quaternion Quaternion::conjugate() {
	return Quaternion(this->t, -this->x, -this->y, -this->z);
}

Quaternion Quaternion::inverse() {
	float quadrance = getQuadrance();
	return (quadrance == 0) ? Quaternion() : conjugate() / getQuadrance();
}

Quaternion Quaternion::lerp(Quaternion& quaternion, float percentage) {
	float cos_angle = this->x*quaternion.x + this->y*quaternion.y + this->z*quaternion.z + this->t*quaternion.t;
	float k0 = 1.0f - percentage;
	float k1 = (cos_angle > 0) ? percentage : -percentage;
	return ((*this)*k0 + quaternion*k1).getNormalized();
}

Quaternion Quaternion::slerp(Quaternion& quaternion, float percentage) {
	float angle = acos(this->x*quaternion.x + this->y*quaternion.y + this->z*quaternion.z + this->t*quaternion.t);
	float k0 = sin((1 - percentage)*angle) / sin(angle);
	float k1 = sin(percentage*angle) / sin(angle);
	return ((*this)*k0 + quaternion*k1).getNormalized();
}


Matrix4 Quaternion::toMatrix() {

	Quaternion qn = getNormalized();

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	return Matrix4(
	1.0f - 2.0f * (yy + zz),	2.0f * (xy + zt),			2.0f * (xz - yt),			0.0f,
	2.0f * (xy - zt),			1.0f - 2.0f * (xx + zz),	2.0f * (yz + xt),			0.0f,
	2.0f * (xz + yt),			2.0f * (yz - xt),			1.0f - 2.0f * (xx + yy),	0.0f,
	0.0f,						0.0f,						0.0f,						1.0f);
	
}

//Unary operators
Quaternion Quaternion::operator- () {
	return -1 * (*this);
}

Quaternion Quaternion::operator+ () {
	return 1 * (*this);
}

//Quaternion with quaternion operations
void Quaternion::operator=(const Quaternion& quaternion) {
	set(quaternion);
}

bool Quaternion::operator!=(const Quaternion& quaternion) {
	return (
		fabs(this->t - quaternion.t) > qThreshold ||
		fabs(this->x - quaternion.x) > qThreshold ||
		fabs(this->y - quaternion.y) > qThreshold ||
		fabs(this->z - quaternion.z) > qThreshold);
}
bool Quaternion::operator==(const Quaternion& quaternion) {
	return (
		fabs(this->t - quaternion.t) < qThreshold &&
		fabs(this->x - quaternion.x) < qThreshold &&
		fabs(this->y - quaternion.y) < qThreshold &&
		fabs(this->z - quaternion.z) < qThreshold);
}

Quaternion Quaternion::operator+(const Quaternion& quaternion) {
	return Quaternion(this->t + quaternion.t, this->x + quaternion.x, this->y + quaternion.y, this->z + quaternion.z);
}

Quaternion Quaternion::operator-(const Quaternion& quaternion) {
	return Quaternion(this->t - quaternion.t, this->x - quaternion.x, this->y - quaternion.y, this->z - quaternion.z);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) {
	return Quaternion(
		this->t * quaternion.t - this->x * quaternion.x - this->y * quaternion.y - this->z * quaternion.z,
		this->t * quaternion.x + this->x * quaternion.t + this->y * quaternion.z - this->z * quaternion.y,
		this->t * quaternion.y + this->y * quaternion.t + this->z * quaternion.x - this->x * quaternion.z,
		this->t * quaternion.z + this->z * quaternion.t + this->x * quaternion.y - this->y * quaternion.x);
}

Quaternion Quaternion::operator+=(const Quaternion& quaternion) {
	set(this->t + quaternion.t, this->x + quaternion.x, this->y + quaternion.y, this->z + quaternion.z);
	return (*this);
}

Quaternion Quaternion::operator-=(const Quaternion& quaternion) {
	set(this->t - quaternion.t, this->x - quaternion.x, this->y - quaternion.y, this->z - quaternion.z);
	return (*this);
}
Quaternion Quaternion::operator*=(const Quaternion& quaternion) {
	set(
		this->t * quaternion.t - this->x * quaternion.x - this->y * quaternion.y - this->z * quaternion.z,
		this->t * quaternion.x + this->x * quaternion.t + this->y * quaternion.z - this->z * quaternion.y,
		this->t * quaternion.y + this->y * quaternion.t + this->z * quaternion.x - this->x * quaternion.z,
		this->t * quaternion.z + this->z * quaternion.t + this->x * quaternion.y - this->y * quaternion.x);
	return (*this);
}

//Quaternion with scalar operations
Quaternion Quaternion::operator*(const float scalar) {
	return Quaternion(this->t*scalar, this->x*scalar, this->y*scalar, this->z*scalar);
}

Quaternion Quaternion::operator/(const float scalar) {
	return Quaternion(this->t/scalar, this->x/scalar, this->y/scalar, this->z/scalar);
}

Quaternion Quaternion::operator*=(const float scalar) {
	set(this->t*scalar, this->x*scalar, this->y*scalar, this->z*scalar);
	return (*this);
}

Quaternion Quaternion::operator/=(const float scalar) {
	set(this->t / scalar, this->x / scalar, this->y / scalar, this->z / scalar);
	return (*this);
}

//print as angle axis
void Quaternion::printAsAngleAxis() {


	float thetaf;
	Vector3 axis_f;
	toAngleAxis(thetaf, axis_f);

	std::cout << "[angle = " << thetaf << ", " << " axis = " << axis_f << "]\n";
}

Quaternion operator*(const float scalar, Quaternion& quaternion) {
	return quaternion*scalar;
}

//Matrix printing
std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion) {
	return out << "( " << quaternion.t << ", " << 
		quaternion.x << ", " << quaternion.y << ", " << quaternion.z << " )";
}


///////////////////////////////////////////////////////////////////////
/*
#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;

void qtest1()
{
	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")

	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
	qtrn q = qFromAngleAxis(90.0f, axis);
	qPrint("   q", q);

	qtrn vi = {0.0f, 7.0f, 0.0f, 0.0f};
	qPrint("  vi", vi);

	qtrn qe = {0.0f, 0.0f, 0.0f, -7.0f};
	qPrint("  qe", qe);

	qtrn qinv = qInverse(q);
	qClean(qinv);
	qPrint("qinv", qinv);

	qtrn qf = qMultiply(qMultiply(q,vi), qinv);
	qPrint("  qf", qf);

	assert(qEqual(qf, qe));
}

void qtest2()
{
	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")

	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
	qtrn q = qFromAngleAxis(90.0f, axis);
	qPrint(" q", q);

	vec4 vi = {7.0f, 0.0f, 0.0f, 1.0f};
	vPrint("vi", vi);

	vec4 ve = {0.0f, 0.0f, -7.0f, 1.0f};
	vPrint("ve", ve);

	mat4 m;
	qGLMatrix(q,m);
	mPrint(" M", m);

	vec4 vf = mMultiply(m, vi);
	vPrint("vf", vf);

	assert(vEqual(vf, ve));
}

void qtest3()
{
	HEADER("TEST 3 : Yaw 180 = Roll 180 + Pitch 180")

	vec4 axis_x = {1.0f, 0.0f, 0.0f, 1.0f};
	vec4 axis_y = {0.0f, 1.0f, 0.0f, 1.0f};
	vec4 axis_z = {0.0f, 0.0f, 1.0f, 1.0f};

	qtrn qyaw180 = qFromAngleAxis(900.0f, axis_y);
	qPrint("  qyaw180", qyaw180);

	qtrn qroll180 = qFromAngleAxis(180.0f, axis_x);
	qPrint(" qroll180", qroll180);

	qtrn qpitch180 = qFromAngleAxis(180.0f, axis_z);
	qPrint("qpitch180", qpitch180);

	qtrn qrp = qMultiply(qpitch180, qroll180);
	qPrint("      qrp", qrp);

	qtrn qpr = qMultiply(qroll180, qpitch180);
	qPrint("      qpr", qpr);

	qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	qPrint("       qi", qi);

	qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	qPrint("       qe", qe);

	qtrn qrpinv = qInverse(qrp);
	qtrn qfrp = qMultiply(qMultiply(qrp, qi), qrpinv);
	qPrint("     qfrp", qfrp);

	qtrn qprinv = qInverse(qpr);
	qtrn qfpr = qMultiply(qMultiply(qpr, qi), qprinv);
	qPrint("     qfpr", qfpr);

	assert(qEqual(qe, qfrp));
	assert(qEqual(qe, qfpr));
}

void qtest4()
{
	HEADER("TEST 4: Q <-> (angle, axis)")

	float thetai = 45.0f;
	vec4 axis_i = {0.0f, 1.0f, 0.0f, 1.0f};
	qtrn q = qFromAngleAxis(thetai, axis_i);
	std::cout << thetai << " "; 
	vPrint("axis_i", axis_i);

	float thetaf;
	vec4 axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << thetaf << " "; 
	vPrint("axis_f", axis_f);
	
	assert(fabs(thetai - thetaf) < qThreshold && vEqual(axis_i, axis_f)); 
}

void qtest5()
{
	HEADER("TEST 5: LERP")

	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
	qtrn q0 = qFromAngleAxis(0.0f, axis);
	qPrint("       q0", q0);

	qtrn q1 = qFromAngleAxis(90.0f, axis);
	qPrint("       q1", q1);

	qtrn qe = qFromAngleAxis(30.0f, axis);
	qPrint("       qe", qe);

	qtrn qLerp0 = qLerp(q0, q1, 0.0f);
	qPrint("  lerp(0)", qLerp0);
	assert(qEqual(qLerp0, q0));

	qtrn qLerp1 = qLerp(q0, q1, 1.0f);
	qPrint("  lerp(1)", qLerp1);
	assert(qEqual(qLerp1, q1));

	qtrn qlerp = qLerp(q0, q1, 1/3.0f);
	qPrint("lerp(1/3)", qlerp);
	qPrintAngleAxis("lerp(1/3)", qlerp);

	assert(qEqual(qlerp, qe) == 0);
}

void qtest6()
{
	HEADER("TEST 6: SLERP")

	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
	qtrn q0 = qFromAngleAxis( 0.0f, axis);
	qPrint("        q0", q0);

	qtrn q1 = qFromAngleAxis(90.0f, axis);
	qPrint("        q1", q1);

	qtrn qe = qFromAngleAxis(30.0f, axis);
	qPrint("        qe", qe);

	qtrn qSlerp0 = qSlerp(q0, q1, 0.0f);
	qPrint("  slerp(0)", qSlerp0);
	assert(qEqual(qSlerp0, q0));

	qtrn qSlerp1 = qSlerp(q0, q1, 1.0f);
	qPrint("  slerp(1)", qSlerp1);
	assert(qEqual(qSlerp1, q1));

	qtrn qslerp = qSlerp(q0, q1, 1/3.0f);
	qPrint("slerp(1/3)", qslerp);
	qPrintAngleAxis("slerp(1/3)", qslerp);

	assert(qEqual(qslerp, qe));
}

///////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	qtest1();
	qtest2();
	qtest3();
	qtest4();
	qtest5();
	qtest6();

	exit(EXIT_SUCCESS);
}
*/

///////////////////////////////////////////////////////////////////////