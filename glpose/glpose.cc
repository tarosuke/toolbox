/*************************** OpenGL pose adapter of quoternion & vector:glpose
 *
 */
#include <GL/gl.h>
#include <math.h>

#include "glpose.h"



COMPLEX<4> GLPOSE::GetDirection() const{
	COMPLEX<4> d(-directionOffset);
	d *= direction;
	return d;
}

void GLPOSE::GetModel() const{
	//移動
	const double* const v(position);
	glTranslatef(v[0], v[1], v[2]);

	//回転
	const COMPLEX<4>::ROTATION r(GetDirection());
	glRotated(r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);
}

void GLPOSE::GetView() const{
	//回転
	const COMPLEX<4>::ROTATION r(GetDirection());
	glRotated(-r.angle * 180 / M_PI, r.axis[0], r.axis[1], r.axis[2]);

	//移動
// 	const double* const v(position);
// 	glTranslatef(-v[0], -v[1], -v[2]);
}

//方位角リセット(絶対座標系)
void GLPOSE::ResetAzimuthOffset(double ratio, unsigned axis){
	COMPLEX<4> o(GetDirection());
	o.FilterAxis(axis); //指定軸だけを残す
	o *= ratio;
	directionOffset *= o;
}

//方位角回転(絶対座標系で回転)
void GLPOSE::RotateAzimuth(const COMPLEX<4>& r){
	directionOffset *= r;
}
