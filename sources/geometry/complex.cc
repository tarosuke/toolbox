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

		const T n(p.Length());
		const T rv[3] = {p[1] / n, p[2] / n, p[3] / n};
		return TB::Vector<3, T>(rv);
	}
	template <typename T> TB::Vector<3, T>
	ReverseRotate(const TB::Complex<4, T>& er, TB::Vector<3, T>& v) {
		TB::Complex<4, T> p(-er);
		TB::Complex<4, T> q(v);
		TB::Complex<4, T> r(er);

		p *= q;
		p *= r;

		const T n(p.Length());
		const T rv[3] = {p[1] / n, p[2] / n, p[3] / n};
		return TB::Vector<3, T>(rv);
	}

	// 乗算演算子
	template <typename T> TB::Complex<2, T>
	Mul(const TB::Complex<2, T>& v, const TB::Complex<2, T>& r) {
		return TB::Complex<2, T>(
			{v[0] * r[0] - v[1] * r[1], v[0] * r[1] + v[1] * r[0]});
	}

	template <typename T> TB::Complex<4, T>
	Mul(const TB::Complex<4, T>& v, const TB::Complex<4, T>& r) {
		return TB::Complex<4, T>(
			{v[0] * r[0] - v[1] * r[1] - v[2] * r[2] - v[3] * r[3],
			 v[1] * r[0] + v[0] * r[1] + v[2] * r[3] - v[3] * r[2],
			 v[2] * r[0] + v[0] * r[2] + v[3] * r[1] - v[1] * r[3],
			 v[3] * r[0] + v[0] * r[3] + v[1] * r[2] - v[2] * r[1]});
	}
}

namespace TB {

#define C(D, T)                                                                \
	template <> Complex<D, T>::Complex(                                        \
		const Vector<D - 1, T>& from,                                          \
		const Vector<D - 1, T>& to) {                                          \
		Diff(value, from, to);                                                 \
		Normalize();                                                           \
	}
	C(4, double)
	C(4, float)
	C(4, int)

#define RnR(D, T)                                                              \
	template <> void Complex<D, T>::Rotate(Vector<D - 1, T>& v) const {        \
		::Rotate(*this, v);                                                    \
	}                                                                          \
	template <> void Complex<D, T>::ReverseRotate(Vector<D - 1, T>& v) const { \
		::ReverseRotate(*this, v);                                             \
	}
	RnR(4, double) RnR(4, float) RnR(4, int)

#define MUL(D, T)                                                              \
	template <> Complex<D, T> Complex<D, T>::operator*(const Complex<D, T>& b) \
		const {                                                                \
		return ::Mul(*this, b);                                                \
	}
		;
	MUL(2, double);
	MUL(2, float);
	MUL(4, double);
	MUL(4, float);
	MUL(2, int);
	MUL(4, int);
}
