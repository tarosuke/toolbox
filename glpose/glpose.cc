/*************************** OpenGL pose adapter of quoternion & vector:glpose
 *
 */
#include <GL/gl.h>
#include <math.h>

#include "glpose.h"


void GLPOSE::GetModel() const{
	//移動
	const double* const v(position);
	glTranslatef(v[0], v[1], v[2]);

	//回転
	const COMPLEX<4>::ROTATION r(direction);
	glRotated(r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);
}

void GLPOSE::GetView() const{
	//回転
	glScalef(1, 2, 1); //TODO:VIEWへ移動する必要がある
	const COMPLEX<4>::ROTATION r(direction);
	glRotated(-r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);

	//移動
	const double* const v(position);
	glTranslatef(-v[0], -v[1], -v[2]);
}

//方位角リセット(絶対座標系)
void GLPOSE::ResetAzimuth(double ratio, unsigned axis){
	COMPLEX<4> o(-direction);
	o *= ratio;
	o.FilterAxis(axis); //指定軸だけを残す
	o *= direction;
	direction = o;
}

//方位角回転(絶対座標系で回転)
void GLPOSE::RotateAzimuth(const COMPLEX<4>& r){
	COMPLEX<4> o(-r);
	o *= direction;
	direction = o;
}
