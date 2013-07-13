/****************************************** OpenGL adapter of quoternion:glqon
 *
 */
#include <GL/gl.h>
#include <math.h>

#include <qon/glqon.h>


void GLQON::GetView(){
	QON::ROTATION r;
	qon.GetRotation(r);
	glRotated(-r.angle * 180 / M_PI, r.x, r.y, r.z);
}

void GLQON::GetModel(){
	QON::ROTATION r;
	qon.GetRotation(r);
	glRotated(r.angle * 180 / M_PI, r.x, r.y, r.z);
}


void GLVQON::GetView(){
	glTranslatef(-vqon.i, -vqon.j, -vqon.k);
}

void GLVQON::GetModel(){
	glTranslatef(vqon.i, vqon.j, vqon.k);
}

