/****************************************** OpenGL adapter of quoternion:glqon
 *
 */
#include <GL/gl.h>
#include <math.h>

#include <qon/glqon.h>


void GLQON::GetView() const{
	glScalef(1, 2, 1);
	QON::ROTATION r;
	qon.GetRotation(r);
	glRotated(-r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);
}

void GLQON::GetModel() const{
	QON::ROTATION r;
	qon.GetRotation(r);
	glRotated(r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);
}


void GLVQON::GetView() const{
	const double* const v(vqon);
	glTranslatef(-v[0], -v[1], -v[2]);
}

void GLVQON::GetModel() const{
	const double* const v(vqon);
	glTranslatef(v[0], v[1], v[2]);
}

