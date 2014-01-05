/*************************** OpenGL pose adapter of quoternion & vector:glpose
 *
 */
#pragma once

#include "../complex/complex.h"


class GLPOSE{
public:
	//GLへ反映
	void GetModel() const;
	void GetView() const;
	//取得系
	const COMPLEX<4>& GetDirection(){ return direction; };
	//回転操作
	void operator=(const COMPLEX<4>& d){ direction = d; }; //機体座標系
	void Rotate(const COMPLEX<4>& r){ direction *= r; };
	void RotateAzimuth(const COMPLEX<4>& r); //絶対座標系
	void ResetAzimuth(double ratio = 1.0, unsigned axis = 2);
	//移動操作
	void MoveTo(const VECTOR<3>& p){ position = p; };
	void Move(const VECTOR<3>& v){ position += v; };
private:
	COMPLEX<4> direction;
	VECTOR<3> position;
};

