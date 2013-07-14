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
	glRotated(-r.angle * 180 / M_PI, r.x, r.y, r.z);
}

void GLQON::GetModel() const{
	QON::ROTATION r;
	qon.GetRotation(r);
	glRotated(r.angle * 180 / M_PI, r.x, r.y, r.z);
}


void GLVQON::GetView() const{
	glTranslatef(-vqon.i, -vqon.j, -vqon.k);
}

void GLVQON::GetModel() const{
	glTranslatef(vqon.i, vqon.j, vqon.k);
}

