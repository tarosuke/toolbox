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
	//オフセット操作回転(絶対座標系)
	void ResetAzimuthOffset(double ratio = 1.0, unsigned axis = 2);
	void RotateOffset(const COMPLEX<4>&){  };
	//オフセット移動操作(絶対座標系)
	void MoveOffsetTo(const VECTOR<3>& v){ positionOffset = v; };
	COMPLEX<4> GetDirection() const;
protected:
	//実際の回転操作
	void operator=(const COMPLEX<4>& d){ direction = d; }; //機体座標系
	void Rotate(const COMPLEX<4>& r){ direction *= r; };
	void RotateAzimuth(const COMPLEX<4>& r); //絶対座標系
	//実際の移動操作
	void MoveTo(const VECTOR<3>& p){ position = p; };
private:
	COMPLEX<4> direction;
	VECTOR<3> position;
	COMPLEX<4> directionOffset;
	VECTOR<3> positionOffset;
};

