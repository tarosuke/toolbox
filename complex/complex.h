/********************************************************************* COMPLEX
 * 一般化した複素数クラスとベクタ
 * NOTE:COMPLEXはdim=2,4の時のみ乗算や回転が可能
 */

#ifndef _COMPLEX_
#define _COMPLEX_

#include <math.h>


template<unsigned dim> class COMPLEX{
private:
	static const unsigned D = dim - 1;
public:
	COMPLEX(){ InitIdentity(); };

	//各軸の回転角で初期化
	template<typename X, typename Y>
	  COMPLEX(const X axis[D], Y ratio = 1){
		//一旦格納
		for(unsigned n(0); n < D; ++n){
			a[n] = axis[n] * ratio;
		}
		//回転角を算出
		double norm2(0.0);
		for(unsigned n(0); n < D; ++n){
			norm2 += a[n] * a[n];
		}
		if(0.0 < norm2){
			//回転角が0.0ではない場合は普通にn元数算出
			const double norm(sqrt(norm2));
			const double half(norm * 0.5);
			const double sinA(sin(half) / norm);
			R = cos(half);
			for(unsigned n(0); n < D; ++n){
				a[n] *= sinA;
			}
		}else{
			//norm2が0.0なら回転なし(a[1]〜a[dim-1]はすべて0)
			R = 1.0;
		}
	};

	//自由軸回転
	struct ROTATION{
		double angle;
		double axis[D];
	};
	void GetRotation(ROTATION& r) const{
		const double s(sqrt(1 - R*R));
		r.angle = acos(R) *2;
		for(unsigned n(0); n < D; ++n){
			r.axis[n] = a[n] / s;
		}
	};
	operator ROTATION() const{
		ROTATION r;
		GetRotation(r);
		return r;
	};
	void operator =(const ROTATION& rot){
		const double ha(rot.angle * 0.5);
		const double s(sin(ha));
		R = cos(ha);
		for(unsigned n(0); n < D; ++n){
			a[D] = rot.axis[n] * s;
		}
	};

	//ノルム算出
	double Norm(){
		double n2(R * R);
		for(unsigned n(0); n < D; ++n){
			n2 += a[n] * a[n];
		}
		return sqrt(n2);
	};

	//ノルムを1.0にする
	void Normalize(){
		const double r(1.0 / Norm());
		R *= r;
		for(unsigned n(0); n < D; ++n){
			a[n] *= r;
		}
	};

	//算術オペレータ
	const COMPLEX operator -() const{ //共役を求める
		COMPLEX t;
		t.R = R;
		for(unsigned n(0); n < D; ++n){
			t.a[n] = -a[n];
		}
		return t;
	};
	const COMPLEX& operator *=(const COMPLEX&); //乗算
protected:
	//データ(R;a[]になってる)
	double R; //実部
	double a[D]; //虚部

	//VECTORのためのメソッド
	COMPLEX(unsigned) : R(0.0){
		for(unsigned n(0); n < D; a[n++] = 0.0);
	};
	template<typename X>
	  COMPLEX(const X axis[D], double ratio, unsigned){
		R = 0;
		for(unsigned n(0); n < D; ++n){
			a[n] = axis[n] * ratio;
		}
	};
	void CopyFrom(const COMPLEX& from){
		R = 0.0;
		for(unsigned n(0); n < D; ++n){
			a[n] = from.a[n];
		}
	};
private:
	void InitIdentity(){
		R = 1.0;
		for(unsigned n(0); n < D; a[n++] = 0.0);
	};
};



template<unsigned dim> class VECTOR : private COMPLEX<dim + 1>{
	//NOTE:親クラスで配列が一つ多く確保されていることに注意
	const VECTOR& operator*=(VECTOR&);
private:
	typedef COMPLEX<dim + 1> C;
public:
	VECTOR() : C(0){};
	template<typename X> VECTOR(const X axis[dim], double ratio = 1.0) :
	    C(axis, ratio, 0){};

	//COMPLEXによる操作(3次元のみ)
	void Rotate(const C& er);
	void ReverseRotate(const C& er);
	//状態取得
	double Length(){
		double n2(0);
		for(unsigned n(0); n < dim; ++n){
			n2 += C::a[n] * C::a[n];
		}
		return sqrt(n2);
	};
	void Normalize(){
		const double r(1.0 / Length());
		C::R = 0;
		for(unsigned n(0); n < dim; n++){
			C::a[n] *= r;
		}
	};
	//算術オペレータ
	double operator*(const VECTOR& r) const{ //内積
		double in(0);
		for(unsigned n(0); n < dim; ++n){
			in += C::a[n] * C::r.a[n];
		}
		return in;
	};
	VECTOR Ex(const VECTOR& t) const; //外積
	double Cross(const VECTOR& t) const; //外積(2次元専用)
	const VECTOR& operator+=(const VECTOR& t){
		for(unsigned n(0); n < dim; ++n){
			C::a[n] += (C::t).a[n];
		}
		return *this;
	};
	const VECTOR& operator-=(const VECTOR& t){
		for(unsigned n(0); n < dim; ++n){
			C::a[n] -= (C::t).a[n];
		}
		return *this;
	};
	VECTOR& operator+(const VECTOR& t) const{
		VECTOR r(*this);
		r += t;
		return r;
	};
	VECTOR& operator-(const VECTOR& t) const{
		VECTOR r(*this);
		r -= t;
		return r;
	};
	void Max(const VECTOR& t){
		for(unsigned n(0); n < dim; ++n){
			if(C::a[n] < (C::t).a[n]){
				C::a[n] = (C::t).a[n];
			}
		}
	};
	void Min(const VECTOR& t){
		for(unsigned n(0); n < dim; ++n){
			if(C::a[n] > (C::t).a[n]){
				C::a[n] = (C::t).a[n];
			}
		}
	};
	const VECTOR operator*=(double t){
		for(unsigned n(0); n < dim; ++n){
			C::a[n] *= t;
		}
		return *this;
	};
private:
};


#endif
