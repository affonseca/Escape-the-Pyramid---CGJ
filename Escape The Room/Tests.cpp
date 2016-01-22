#include "Tests.h"

void printVector2Test() {

	Vector2 v1(3.0f, 8.0f), v2(5.0f, 2.0f), v3(10.0f, -3.0f);
	Vector2 v1Copy(0.0f, 0.0f), v2Copy(v2), v3Copy(v3);

	std::cout << "VECTOR 2:\n\nVector with vector operations:\n"
		<< v1 << '+' << v2 << '=' << v1 + v2 << '\n'
		<< v2 << '-' << v3 << '=' << v2 - v3 << '\n'
		<< v3 << '*' << v1 << " cannot be done. Operation invalid.\n"
		<< v2 << '.' << v3 << " cannot be done.Operation invalid.\n\n";

	std::cout << "Vector assignments (using vectors):\n"
		<< "assingned " << v1Copy << " as " << v1 << '=';
	v1Copy = v1;
	std::cout << v1Copy << '\n';

	v1Copy += v2;
	v2Copy -= v3;

	std::cout
		<< "assingned " << v1 << " as " << v1 << '+' << v2 << '=' << v1Copy << '\n'
		<< "assingned " << v2 << " as " << v2 << '-' << v3 << '=' << v2Copy << "\n\n";

	std::cout << "Vector with scalar operations:\n"
		<< v1 << "*3 = " << v1 * 3 << '\n'
		<< "-4*" << v2 << " = " << -4 * v2 << '\n'
		<< v3 << "/2 = " << v3 / 2 << '\n'
		<< "-6/" << v1 << " cannot be done. Operation invalid.\n\n";

	v1Copy = v1; v3Copy = v3;
	v1Copy *= 3;
	v3Copy /= 2;

	std::cout << "Vector assignments (using scalars):\n"
		<< "assingned " << v1 << " as " << v1 << "*3 = " << v1Copy << '\n'
		<< "assingned " << v3 << " as " << v3 << "/2 = " << v3Copy << "\n\n";

	v2Copy = v2; v2Copy.normalize();
	std::cout << "Norm operations:\n"
		<< v2 << " norm is " << v2.getNorm() << '\n'
		<< v2 << " normalized is " << v2.getNormalized() << '\n'
		<< v2 << " set as normalized is " << v2Copy << "\n"
		<< v2Copy << " norm is " << v2Copy.getNorm() << "\n\n";

	v1Copy.set(50.0f, 50.0f);
	v2Copy.set(50.0f, 0.0f);
	v3Copy.set(-150.0f, -150.0f);
	std::cout << "Angle between vector:\n"
		<< "Angle between" << v1Copy << " and " << v2Copy << " is " << v1Copy.angleWithVector(v2Copy) << " degrees \n"
		<< "Angle between" << v1Copy << " and " << v3Copy << " is " << v1Copy.angleWithVector(v3Copy) << " degrees \n"
		<< "Angle between" << v2Copy << " and " << v3Copy << " is " << v2Copy.angleWithVector(v3Copy) << " degrees \n\n";


	std::cout << "Projections on vectors:\n"
		<< "Projection of" << v1Copy << " on " << v2Copy << " is " << v1Copy.projectOn(v2Copy) << "\n"
		<< "Projection of" << v1Copy << " on " << v3Copy << " is " << v1Copy.projectOn(v3Copy) << "\n"
		<< "Projection of" << v2Copy << " on " << v1Copy << " is " << v2Copy.projectOn(v1Copy) << "\n"
		<< "Projection of" << v2Copy << " on " << v3Copy << " is " << v2Copy.projectOn(v3Copy) << "\n"
		<< "Projection of" << v3Copy << " on " << v1Copy << " is " << v3Copy.projectOn(v1Copy) << "\n"
		<< "Projection of" << v3Copy << " on " << v2Copy << " is " << v3Copy.projectOn(v2Copy) << "\n\n";

	std::cout << "Linear interpolation between vectors:\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "25%: " << v3Copy.lerp(v1Copy, 0.25) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "50%: " << v3Copy.lerp(v1Copy, 0.5) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "75%: " << v3Copy.lerp(v1Copy, 0.75) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "87.5%: " << v3Copy.lerp(v1Copy, 0.875) << "\n\n\n";

}

void printVector3Test() {
	Vector3 v1(3.0f, 8.0f, 4.0f), v2(5.0f, 2.0f, -3.0f), v3(10.0f, -3.0f, 7.0f);
	Vector3 v1Copy(0.0f, 0.0f, 0.0f), v2Copy(v2), v3Copy(v3);

	std::cout << "VECTOR 3:\n\nVector with vector operations:\n"
		<< v1 << '+' << v2 << '=' << (v1 + v2) << '\n'
		<< v2 << '-' << v3 << '=' << v2 - v3 << '\n'
		<< v3 << '*' << v1 << '=' << v3*v1 << '\n'
		<< v2 << '.' << v3 << '=' << v2.dot(v3) << "\n\n";

	std::cout << "Vector assignments (using vectors):\n"
		<< "assingned " << v1Copy << " as " << v1 << '=';
	v1Copy = v1;
	std::cout << v1Copy << '\n';

	v1Copy += v2;
	v2Copy -= v3;
	v3Copy *= v1;

	std::cout
		<< "assingned " << v1 << " as " << v1 << '+' << v2 << '=' << v1Copy << '\n'
		<< "assingned " << v2 << " as " << v2 << '-' << v3 << '=' << v2Copy << '\n'
		<< "assingned " << v3 << " as " << v3 << '*' << v1 << '=' << v3Copy << "\n\n";

	std::cout << "Vector with scalar operations:\n"
		<< v1 << "*3 = " << v1 * 3 << '\n'
		<< "-4*" << v2 << " = " << -4 * v2 << '\n'
		<< v3 << "/2 = " << v3 / 2 << '\n'
		<< "-6/" << v1 << " cannot be done. Operation invalid.\n\n";

	v1Copy = v1; v3Copy = v3;
	v1Copy *= 3;
	v3Copy /= 2;

	std::cout << "Vector assignments (using scalars):\n"
		<< "assingned " << v1 << " as " << v1 << "*3 = " << v1Copy << '\n'
		<< "assingned " << v3 << " as " << v3 << "/2 = " << v3Copy << "\n\n";

	v2Copy = v2; v2Copy.normalize();
	std::cout << "Norm operations:\n"
		<< v2 << " norm is " << v2.getNorm() << '\n'
		<< v2 << " normalized is " << v2.getNormalized() << '\n'
		<< v2 << " set as normalized is " << v2Copy << "\n"
		<< v2Copy << " norm is " << v2Copy.getNorm() << "\n\n";

	v1Copy.set(50.0f, 50.0f, 0.0f);
	v2Copy.set(50.0f, 0.0f, 0.0f);
	v3Copy.set(-150.0f, -150.0f, 0.0f);
	std::cout << "Angle between vector:\n"
		<< "Angle between" << v1Copy << " and " << v2Copy << " is " << v1Copy.angleWithVector(v2Copy) << " degrees \n"
		<< "Angle between" << v1Copy << " and " << v3Copy << " is " << v1Copy.angleWithVector(v3Copy) << " degrees \n"
		<< "Angle between" << v2Copy << " and " << v3Copy << " is " << v2Copy.angleWithVector(v3Copy) << " degrees \n\n";


	std::cout << "Projections on vectors:\n"
		<< "Projection of" << v1Copy << " on " << v2Copy << " is " << v1Copy.projectOn(v2Copy) << "\n"
		<< "Projection of" << v1Copy << " on " << v3Copy << " is " << v1Copy.projectOn(v3Copy) << "\n"
		<< "Projection of" << v2Copy << " on " << v1Copy << " is " << v2Copy.projectOn(v1Copy) << "\n"
		<< "Projection of" << v2Copy << " on " << v3Copy << " is " << v2Copy.projectOn(v3Copy) << "\n"
		<< "Projection of" << v3Copy << " on " << v1Copy << " is " << v3Copy.projectOn(v1Copy) << "\n"
		<< "Projection of" << v3Copy << " on " << v2Copy << " is " << v3Copy.projectOn(v2Copy) << "\n\n";

	std::cout << "Linear interpolation between vectors:\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "25%: " << v3Copy.lerp(v1Copy, 0.25) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "50%: " << v3Copy.lerp(v1Copy, 0.5) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "75%: " << v3Copy.lerp(v1Copy, 0.75) << "\n"
		<< "Lerp from " << v3Copy << " to " << v1Copy << " at " << "87.5%: " << v3Copy.lerp(v1Copy, 0.875) << "\n\n";
}

void printMatrix2Test() {

	Matrix2 m1(3.0f, 8.0f, 0.5f, 12.0f), m2(5.0f, 2.0f, -13.0f, 21.0f), m3(10.0f, -3.0f, -5.0f, 4.0f);
	Matrix2 m1Copy(ZEROS_MATRIX), m2Copy(m2), m3Copy(m3);

	std::cout << "MATRIX 2x2:\n\nMatrix with matrix operations:\n\n\n"
		<< m1 << "\n+\n\n" << m2 << "\n=\n\n" << m1 + m2 << "\n\n\n"
		<< m2 << "\n-\n\n" << m3 << "\n=\n\n" << m2 - m3 << "\n\n\n"
		<< m3 << "\n*\n\n" << m1 << "\n=\n\n" << m3 * m1 << "\n\n\n";

	std::cout << "Matrix assignments (using matrices):\n"
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n=\n\n\n";
	m1Copy = m1;
	std::cout << m1Copy << '\n';

	m1Copy += m2;
	m2Copy -= m3;
	m3Copy *= m1;

	std::cout
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n+\n\n" << m2 << "\n=\n\n" << m1 + m2 << "\n\n\n"
		<< m2Copy << "\nassingned as\n\n" << m2 << "\n-\n\n" << m3 << "\n=\n\n" << m2 - m3 << "\n\n\n"
		<< m3Copy << "\nassingned as\n\n" << m3 << "\n*\n\n" << m1 << "\n=\n\n" << m3 * m1 << "\n\n\n";

	std::cout << "Matrix with scalar operations:\n"
		<< m1 << "\n* 3 =\n\n" << m1 * 3 << "\n\n\n"
		<< "-4 *\n\n" << m2 << "\n=\n\n" << -4 * m2 << "\n\n\n"
		<< m3 << "\n/ 2 =\n\n" << m3 / 2 << "\n\n\n"
		<< "-6 /\n\n" << m1 << "\ncannot be done. Operation invalid.\n\n\n";

	m1Copy = m1; m3Copy = m3;
	m1Copy *= 3;
	m3Copy /= 2;

	std::cout << "Matrix assignments (using scalars):\n"
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n* 3 =\n\n" << m1 * 3 << "\n\n\n"
		<< m3Copy << "\nassingned as\n\n" << m3 << "\n/ 2 =\n\n" << m3 / 2 << "\n\n\n";

	Vector2 vec = Vector2(-2.2f, 4.3f);
	std::cout << "Matrix with vector operations:\n"
		<< m1 << "\n* " << vec << " = " << m1*vec << "\n\n\n";

	std::cout << "Other operations:\n"
		<< "determinant of\n\n" << m1 << "\n=\n\n" << m1.determinant() << "\n\n\n"
		<< "transpose of\n\n" << m2 << "\n=\n\n" << m2.transpose() << "\n\n\n"
		<< "inverse of\n\n" << m3 << "\n=\n\n" << m3.inverse() << "\n\n\n";

	std::cout << "Identity matrix:\n" << m1 * m1.inverse() << "\n=\n\n" << Matrix2(IDENTITY_MATRIX) << "\n\n\n";

}

void printMatrix3Test() {

	Matrix3
		m1(3.0f, 8.0f, 0.5f, 12.0f, -4.0f, 12.5f, -6.3f, 23.0f, -52.0f),
		m2(5.0f, 2.0f, -13.0f, 21.0f, 32.0f, 64.0f, -34.3f, 645.23f, -54.22f),
		m3(10.0f, -3.0f, -5.0f, 40.0f, 63.0f, 24.54f, 122.0f, 52.0f, -34.0f);
	Matrix3 m1Copy(ZEROS_MATRIX), m2Copy(m2), m3Copy(m3);

	std::cout << "MATRIX 3x3:\n\nMatrix with matrix operations:\n\n\n"
		<< m1 << "\n+\n\n" << m2 << "\n=\n\n" << m1 + m2 << "\n\n\n"
		<< m2 << "\n-\n\n" << m3 << "\n=\n\n" << m2 - m3 << "\n\n\n"
		<< m3 << "\n*\n\n" << m1 << "\n=\n\n" << m3 * m1 << "\n\n\n";

	std::cout << "Matrix assignments (using matrices):\n"
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n=\n\n\n";
	m1Copy = m1;
	std::cout << m1Copy << '\n';

	m1Copy += m2;
	m2Copy -= m3;
	m3Copy *= m1;

	std::cout
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n+\n\n" << m2 << "\n=\n\n" << m1 + m2 << "\n\n\n"
		<< m2Copy << "\nassingned as\n\n" << m2 << "\n-\n\n" << m3 << "\n=\n\n" << m2 - m3 << "\n\n\n"
		<< m3Copy << "\nassingned as\n\n" << m3 << "\n*\n\n" << m1 << "\n=\n\n" << m3 * m1 << "\n\n\n";

	std::cout << "Matrix with scalar operations:\n"
		<< m1 << "\n* 3 =\n\n" << m1 * 3 << "\n\n\n"
		<< "-4 *\n\n" << m2 << "\n=\n\n" << -4 * m2 << "\n\n\n"
		<< m3 << "\n/ 2 =\n\n" << m3 / 2 << "\n\n\n"
		<< "-6 /\n\n" << m1 << "\ncannot be done. Operation invalid.\n\n\n";

	m1Copy = m1; m3Copy = m3;
	m1Copy *= 3;
	m3Copy /= 2;

	std::cout << "Matrix assignments (using scalars):\n"
		<< m1Copy << "\nassingned as\n\n" << m1 << "\n* 3 =\n\n" << m1 * 3 << "\n\n\n"
		<< m3Copy << "\nassingned as\n\n" << m3 << "\n/ 2 =\n\n" << m3 / 2 << "\n\n\n";

	Vector3 vec = Vector3(-2.2f, 4.3f, 2.6f);
	std::cout << "Matrix with vector operations:\n"
		<< m1 << "\n* " << vec << " = " << m1*vec << "\n\n\n";

	std::cout << "Other operations:\n"
		<< "determinant of\n\n" << m1 << "\n=\n\n" << m1.determinant() << "\n\n\n"
		<< "transpose of\n\n" << m2 << "\n=\n\n" << m2.transpose() << "\n\n\n"
		<< "inverse of\n\n" << m3 << "\n=\n\n" << m3.inverse() << "\n\n\n";

	std::cout << "Identity matrix:\n" << m1 * m1.inverse() << "\n=\n\n" << Matrix3(IDENTITY_MATRIX) << "\n\n\n";

}

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;

void qtest1()
{
	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")

	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q = Quaternion(90.0f, axis);
	std::cout << "q = " << q << "\n";

	Quaternion vi = Quaternion(0.0f, 7.0f, 0.0f, 0.0f);
	std::cout << "vi = " << vi << "\n";

	Quaternion qe = Quaternion( 0.0f, 0.0f, 0.0f, -7.0f );
	std::cout << "qe = " << qe << "\n";

	Quaternion qinv = q.inverse();
	qinv.clean();
	std::cout << "qinv = " << qinv << "\n";

	Quaternion qf = (q*vi)*qinv;
	std::cout << "qf = " << qf << "\n";

	assert(qf==qe);
}

void qtest2()
{
	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")

	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q = Quaternion(90.0f, axis);
	std::cout << "q = " << q << "\n";

	Vector3 vi(7.0f, 0.0f, 0.0f);
	std::cout << "vi = " << vi << "\n";

	Vector3 ve(0.0f, 0.0f, -7.0f);
	std::cout << "ve = " << ve << "\n";

	Matrix4 m = q.toMatrix();	
	std::cout << "m = \n" << m << "\n";

	Vector3 vf = m.toMatrix3() * vi;
	vf.clean();
	std::cout << "vf = " << vf << "\n";

	assert(vf==ve);
}

void qtest3()
{
	HEADER("TEST 3 : Yaw 180 = Roll 180 + Pitch 180")

	Vector3 axis_x(1.0f, 0.0f, 0.0f);
	Vector3 axis_y(0.0f, 1.0f, 0.0f);
	Vector3 axis_z(0.0f, 0.0f, 1.0f);

	Quaternion qyaw180 = Quaternion(900.0f, axis_y);
	std::cout << "qyaw180 = " << qyaw180 << "\n";

	Quaternion qroll180 = Quaternion(180.0f, axis_x);
	std::cout << "qroll180 = " << qroll180 << "\n";

	Quaternion qpitch180 = Quaternion(180.0f, axis_z);
	std::cout << "qpitch180 = " << qpitch180 << "\n";

	Quaternion qrp = qpitch180 * qroll180;
	std::cout << "qrp = " << qrp << "\n";

	Quaternion qpr = qroll180 * qpitch180;
	std::cout << "qpr = " << qpr << "\n";

	Quaternion qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	std::cout << "qi = " << qi << "\n";

	Quaternion qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	std::cout << "qe = " << qe << "\n";

	Quaternion qrpinv = qrp.inverse();
	Quaternion qfrp = (qrp*qi)*qrpinv;
	std::cout << "qfrp = " << qfrp << "\n";

	Quaternion qprinv = qpr.inverse();
	Quaternion qfpr = (qpr*qi)*qprinv;
	std::cout << "qfpr = " << qfpr << "\n";

	assert(qe == qfrp);
	assert(qe == qfpr);
}

void qtest4()
{
	HEADER("TEST 4: Q <-> (angle, axis)")

	float thetai = 45.0f;
	Vector3 axis_i(0.0f, 1.0f, 0.0f);
	Quaternion q = Quaternion(thetai, axis_i);
	std::cout << thetai << " ";
	std::cout << "axis_i = " << axis_i << "\n";

	float thetaf;
	Vector3 axis_f;
	q.toAngleAxis(thetaf, axis_f);
	std::cout << thetaf << " ";
	std::cout << "axis_f = " << axis_f << "\n";

	const float qThreshold = (GLfloat)1.0e-5;

	assert(fabs(thetai - thetaf) < qThreshold && axis_i == axis_f);
}

void qtest5()
{
	HEADER("TEST 5: LERP")

	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q0 = Quaternion(0.0f, axis);
	std::cout << "q0 = " << q0 << "\n";

	Quaternion q1 = Quaternion(90.0f, axis);
	std::cout << "q1 = " << q1 << "\n";

	Quaternion qe = Quaternion(30.0f, axis);
	std::cout << "qe = " << qe << "\n";

	Quaternion qLerp0 = q0.lerp(q1, 0.0f);
	std::cout << "lerp(0) = " << qLerp0 << "\n";
	assert(qLerp0 == q0);

	Quaternion qLerp1 = q0.lerp(q1, 1.0f);
	std::cout << "lerp(1) = " << qLerp1 << "\n";
	assert(qLerp1 == q1);

	Quaternion qlerp = q0.lerp(q1, 1.0f / 3.0f);
	std::cout << "lerp(1/3) = " << qlerp << "\n";
	std::cout << "lerp(1/3) = "; qlerp.printAsAngleAxis(); 
	std::cout << "\n";

	assert(!(qlerp == qe));
}

void qtest6()
{
	HEADER("TEST 6: SLERP")

	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q0 = Quaternion(0.0f, axis);
	std::cout << "q0 = " << q0 << "\n";

	Quaternion q1 = Quaternion(90.0f, axis);
	std::cout << "q1 = " << q1 << "\n";

	Quaternion qe = Quaternion(30.0f, axis);
	std::cout << "qe = " << qe << "\n";

	Quaternion qSlerp0 = q0.slerp(q1, 0.0f);
	std::cout << "lerp(0) = " << qSlerp0 << "\n";
	assert(qSlerp0 == q0);

	Quaternion qSlerp1 = q0.slerp(q1, 1.0f);
	std::cout << "lerp(1) = " << qSlerp1 << "\n";
	assert(qSlerp1 == q1);

	Quaternion qslerp = q0.slerp(q1, 1.0f / 3.0f);
	std::cout << "lerp(1/3) = " << qslerp << "\n";
	std::cout << "lerp(1/3) = "; qslerp.printAsAngleAxis();
	std::cout << "\n";

	assert(qslerp == qe);
}