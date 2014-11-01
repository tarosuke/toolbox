/********************************************************************* COMPLEX
 * 一般化した複素数クラスとベクタ
 * NOTE:COMPLEXはdim=2,4の時のみ乗算や回転が可能
 */

#include "complex.h"


template<> const COMPLEX<2>& COMPLEX<2>::operator*=(const COMPLEX<2>& r){
	const double rn(R*r.R - a[0]*r.a[0]);
	a[0] = R*r.a[0] + a[0]*r.R;
	R = rn;
	return *this;
}

template<> const COMPLEX<4>& COMPLEX<4>::operator*=(const COMPLEX<4>& r){
	COMPLEX d(*this);
	R = d.R*r.R - d.a[0]*r.a[0] - d.a[1]*r.a[1] - d.a[2]*r.a[2];
	a[0] = d.a[0]*r.R + d.R*r.a[0] + d.a[1]*r.a[2] - d.a[2]*r.a[1];
	a[1] = d.a[1]*r.R + d.R*r.a[1] + d.a[2]*r.a[0] - d.a[0]*r.a[2];
	a[2] = d.a[2]*r.R + d.R*r.a[2] + d.a[0]*r.a[1] - d.a[1]*r.a[0];
	Normalize();
	return *this;
}



template<> VECTOR<3> VECTOR<3>::Ex(const VECTOR<3>& t) const{ //外積(3次元)
	const double arr[3] = {
		a[1]*t.a[2] - a[2]*t.a[1],
		a[2]*t.a[0] - a[0]*t.a[2],
		a[0]*t.a[1] - a[1]*t.a[0] };
	VECTOR<3> r(arr);
	return r;
};
template<> COMPLEX<4>::COMPLEX(const VECTOR<3>& f, const VECTOR<3>& t){
	VECTOR<3> ff(f);
	VECTOR<3> tt(t);
	ff.Normalize();
	tt.Normalize();

	const double in(tt * ff); //内積(差分の回転角)
	VECTOR<3> ex(tt.Ex(ff)); //外積(差分の回転軸)

	const double c(sqrt(0.5 * (1 + in)));
	const double s(sqrt(0.5 * (1 - in)));
	R = c;
	const double* const ea(ex);
	for(unsigned n(0); n < D; ++n){
		a[n] = s * ea[n];
	}
	Normalize();
}

template<> double VECTOR<2>::Cross(const VECTOR<2>& t) const{ //外積(2次元)
	//2次元の外積は存在しないが、代わりに三次元で計算して三軸目の値を返す
	return a[0]*t.a[1] - a[1]*t.a[0];
};

template<> void VECTOR<3>::Rotate(const VECTOR<3>::C& er){
	C p(er);
	C r(-er);

	p *= *this;
	p *= r;

	CopyFrom(p);
	Normalize();
}
template<> void VECTOR<3>::ReverseRotate(const C& er){
	C r(-er);

	r *= *this;
	r *= er;

	CopyFrom(r);
	Normalize();
}
