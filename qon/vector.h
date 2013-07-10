/********************************************************* 任意次元ベクタ:vector
 *
 *
 */

#ifndef _VECTOR_
#define _VECTOR_

#include <math.h>


template<unsigned dimensions> class VECTOR{
public:
	VECTOR(){
		for(unsigned n(0); n < dimensions; val[n++] = 0);
	};
	VECTOR(const double initialValue[]){
		for(unsigned n(0); n < dimensions; n++){
			val[n] = initialValue[n];
		}
	};
	void operator+=(const VECTOR& t){
		for(unsigned n(0); n < dimensions; n++){
			val[n] += t.val[n];
		}
	};
	VECTOR operator+(const VECTOR& t) const{
		VECTOR r(*this);
		r += t;
		return r;
	};
	void operator-=(const VECTOR& t){
		for(unsigned n(0); n < dimensions; n++){
			val[n] -= t.val[n];
		}
	};
	VECTOR operator-(const VECTOR& t) const{
		VECTOR r(*this);
		r -= t;
		return r;
	};
	void operator*=(double t){
		for(unsigned n(0); n < dimensions; n++){
			val[n] *= t;
		}
	};
	VECTOR operator*(double t) const{
		VECTOR r(*this);
		r *= t;
		return r;
	};
	void operator/=(double t){
		const double rt(1.0 / t);
		for(unsigned n(0); n < dimensions; n++){
			val[n] *= rt;
		}
	};
	VECTOR operator/(double t) const{
		VECTOR r(*this);
		r /= t;
		return r;
	};
	double operator*(const VECTOR& t) const{ //NOTE:DOT multiply
		double r(0.0);
		for(unsigned n(0); n < dimensions; n++){
			r += val[n] * t.val[n];
		}
		return r;
	};
	VECTOR Ex(const VECTOR& t) const{ //NOTE:ONLY 3 == dimensions
		VECTOR r(0);
		r.val[0] = val[1]*t.val[2] - val[2]*t.val[1];
		r.val[1] = val[2]*t.val[0] - val[0]*t.val[2];
		r.val[2] = val[0]*t.val[1] - val[1]*t.val[0];
		return r;
	};
	double Ex2(const VECTOR& t) const{ //NOTE:ONLY 2 == dimensions
		return val[0]*t.val[1] - val[1]*t.val[0];
	};
	double Norm() const{
		double r)0.0);
		for(unsigned n(0); n < dimensions; n++){
			r += val[n] * val[n];
		}
		return sqrt(r);
	};
	void Normalize(){
		const double rn(1.0 / Norm());
		for(unsigned n(0); n < dimensions; n++){
			val[n] *= rn;
		}
	};
protected:
	VECTOR(int){}; //初期化なしのダミーコンストラクタ
	double val[dimensions];
};

#endif
