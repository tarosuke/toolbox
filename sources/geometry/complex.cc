/** Complexes
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <toolbox/geometry/complex.h>

#include <math.h>



namespace {

	// ベクトルfrom→toの四元数を求める
	template <typename T> void
	Diff(T (&r)[4], const TB::Vector<3, T>& from, const TB::Vector<3, T>& to) {
		TB::Vector<3, T> ff(from);
		TB::Vector<3, T> tt(to);
		ff.Normalize();
		tt.Normalize();

		const T in(tt * ff); //内積(差分の回転角)
		TB::Vector<3, T> ex(tt.Cross(ff)); //外積(差分の回転軸)

		const T c(sqrt(0.5 * (1 + in)));
		const T s(sqrt(0.5 * (1 - in)));

		r[0] = c; //実部はc
		for (unsigned n(1); n < 4; ++n) {
			r[n] = s * ex[n - 1];
		}
	}

	//四元数でベクトルを回転
	template <typename T>
	TB::Vector<3, T> Rotate(const TB::Complex<4, T>& er, TB::Vector<3, T>& v) {
		TB::Complex<4, T> p(er);
		TB::Complex<4, T> q(v);
		TB::Complex<4, T> r(-er);

		p *= q;
		p *= r;

		p.Normalize();
		const T(&rv)[3] = &p[1];
		return TB::Vector<3, T>(rv);
	}
	template <typename T> TB::Vector<3, T>
	ReverseRotate(const TB::Complex<4, T>& er, TB::Vector<3, T>& v) {
		TB::Complex<4, T> p(-er);
		TB::Complex<4, T> q(v);
		TB::Complex<4, T> r(er);

		p *= q;
		p *= r;

		p.Normalize();
		return TB::Vector<3, T>(p[1]);
	}

	// 乗算演算子
	template <typename T>
	TB::Complex<2, T> Mul(const TB::Complex<2, T>& v, TB::Complex<2, T>& r) {
		return TB::Complex<2, T>(
			{v[0] * r[0] - v[1] * r[1], v[0] * r[1] + v[1] * r[0]});
	}

	// template <> const COMPLEX<4>& COMPLEX<4>::operator*=(const COMPLEX<4>& r)
	// { 	COMPLEX d(*this); 	R = d.R * r.R - d.a[0] * r.a[0] - d.a[1] *
	// r.a[1]
	// - d.a[2] * r.a[2]; 	a[0] = d.a[0] * r.R + d.R * r.a[0] + d.a[1] * r.a[2]
	// - d.a[2] * r.a[1]; 	a[1] = d.a[1] * r.R + d.R * r.a[1] + d.a[2] * r.a[0]
	// - d.a[0] * r.a[2]; 	a[2] = d.a[2] * r.R + d.R * r.a[2] + d.a[0] * r.a[1]
	// - d.a[1] * r.a[0]; 	Normalize(); 	return *this;
	// }
}

namespace TB {

	template <> Complex<4, float>::Complex(
		const Vector<3, float>& from, const Vector<3, float>& to) {
		Diff(value, from, to);
		Normalize();
	}
	template <> Complex<4, double>::Complex(
		const Vector<3, double>& from, const Vector<3, double>& to) {
		Diff(value, from, to);
		Normalize();
	}

#define RnR(D, T)                                                              \
	template <> void Complex<D, T>::Rotate(Vector<D - 1, T>& v) const {        \
		::Rotate(*this, v);                                                    \
	}                                                                          \
	template <> void Complex<D, T>::ReverseRotate(Vector<D - 1, T>& v) const { \
		::ReverseRotate(*this, v);                                             \
	}

	RnR(4, double) RnR(4, float)
}
