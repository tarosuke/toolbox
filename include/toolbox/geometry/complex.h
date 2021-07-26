/** Complexes
 * Copyright (C) 2017, 2021 tarosuke<webmaster@tarosuke.net>
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
 *
 * dim: dimensions
 *     2: a + ib
 *     4: a + ib + jc + kd
 *
 * NOTE:
 * 演算の中には特定次元でしかできないものがあるので注意 *
 * できない次元数で演算しようとするとリンカエラーになる *
 */
#pragma once

#include <toolbox/geometry/vector.h>



namespace TB {

	template <unsigned D, typename T> class Complex {
	public:
		// コンストラクタ
		Complex() : value{} {};

		// 配列に見えるアクセサ
		T operator[](unsigned n) const { return value[n]; }

		// 配列からの代入演算子、コンストラクタ
		template <typename U> void operator=(const U (&o)[D]) {
			InitFromComplex(o);
		};
		template <typename U> Complex(const U (&o)[D]) { InitFromComplex(o); };

		// Complexからの代入演算子、コンストラクタ
		template <typename U> void operator=(const Complex<D, U>& o) {
			InitFromComplex(o);
		};
		template <typename U> Complex(const Complex<D, U>& o) {
			InitFromComplex(o);
		};

		// Vectorおよび配列からの変換代入演算子、コンストラクタ
		template <typename U> void operator=(const U (&o)[D - 1]) {
			InitFromVector(o);
		};
		template <typename U> void operator=(const Vector<D - 1, U>& o) {
			InitFromVector(o);
		};
		template <typename U> Complex(const Vector<D - 1, U>& o) {
			InitFromVector((const U*)o);
		};

		// ベクトルfrom→toの四元数を求める
		Complex(const Vector<D - 1, T>& from, const Vector<D - 1, T>& to);

		// 演算子
		const Complex operator-() const { //共役を求める
			Complex t;
			t.value[0] = value[0];
			for (unsigned n(0); n < D; ++n) {
				t.value[n] = -value[n];
			}
			return t;
		};
		Complex operator*(const Complex&) const; //乗算
		void operator*=(const Complex& t) { *this = *this * t; };
		void operator*=(T t) { //大きさを乗算
			value[0] = (1.0 - t) + value[0] * t;
			for (unsigned n(1); n < D; ++n) {
				value[n] *= t;
			}
		};
		Complex operator*(T t) const {
			Complex o(*this);
			o *= t;
			return o;
		};

		// 回転
		void Rotate(Vector<D - 1, T>&) const;
		void ReverseRotate(Vector<D - 1, T>&) const;

		// ノルム他
		T Length2() const {
			T l2(0);
			for (unsigned n(0); n < D; ++n) {
				l2 += value[n] * value[n];
			}
			return l2;
		}
		T Length() const { return sqrt(Length2()); };
		void Normalize() {
			const T norm(Length());
			for (auto& v : value) {
				v /= norm;
			}
		};

	private:
		T value[D];

		template <typename U> void InitFromComplex(const U* o) {
			for (unsigned n(0); n < D; ++n) {
				value[n] = (T)o[n];
			}
		};
		template <typename U> void InitFromVector(const U* o) {
			value[0] = 0;
			for (unsigned n(1); n < D; ++n) {
				value[n] = (T)o[n - 1];
			}
		};
	};
}