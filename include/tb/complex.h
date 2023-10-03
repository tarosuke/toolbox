/* Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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
#pragma once

#include <math.h>
#include <tb/vector.h>



namespace tb {

	template <unsigned D, typename T> struct Complex {
		///// 構築子、代入、[]
		Complex() { // Identityに初期化
			a[0] = 1;
			for (unsigned n(1); n < D; ++n) {
				a[n] = 0;
			}
		};
		Complex(const Complex&) = default;

		Complex(const T (&o)[D]) {
			for (uint n(0); n < D; ++n) {
				*const_cast<T*>(&a[n]) = o[n];
			}
		};

		// 各軸の回転角で初期化
		Complex(const T axis[D - 1], T ratio) {
			// 一旦格納
			for (unsigned n(1); n < D; ++n) {
				a[n] = axis[n - 1] * ratio;
			}
			// 回転角を算出
			T norm2(0.0);
			for (unsigned n(1); n < D; ++n) {
				norm2 += a[n] * a[n];
			}
			if (0.0 < norm2) {
				// 回転角が0.0ではない場合は普通にn元数算出
				const T norm(sqrt(norm2));
				const T half(norm * 0.5);
				const T sinA(sin(half) / norm);
				a[0] = cos(half);
				for (unsigned n(1); n < D; ++n) {
					a[n] *= sinA;
				}
			} else {
				// norm2が0.0なら回転なし(a[1]〜a[dim-1]はすべて0)
				a[0] = 1.0;
			}
		};

		template <unsigned E> const Complex& operator=(const T (&o)[E]) {
			for (uint n(0); n < D; ++n) {
				a[n] = o[n];
			}
		};
		template <unsigned E> const Complex& operator=(const Complex<E, T>& o) {
			return *this = o.a;
		};

		T operator[](unsigned n) const { return n < D ? a[n] : 0; };



		// 比較(実数の場合誤差が一定以下なら同値とする)
		bool operator==(const Complex& o) const {
			for (uint n(0); n < D; ++n) {
				if constexpr (std::is_floating_point<T>::value) {
					if (0.000001 < abs(a[n] - o.a[n])) {
						return false;
					}
				} else {
					if (a[n] != o.a[n]) {
						return false;
					}
				}
			}
			return true;
		};
		bool operator!=(const Complex& o) const { return !(*this == o); };


		// ノルム
		T Norm2() const {
			if constexpr (D == 2) {
				return a[0] * a[0] + a[1] * a[1];
			} else if constexpr (D == 4) {
				return a[0] * a[0] + a[1] * a[1] + a[2] + a[2] + a[3] * a[3];
			}
			return 0;
		};
		T Norm() const { return sqrt(Norm2()); };
		void Normalize() { *this /= Norm(); };


		// 対スカラー演算
		void operator*=(T o) {
			for (unsigned n(0); n < D; ++n) {
				a[n] *= o;
			}
		};
		void operator/=(T o) {
			for (unsigned n(0); n < D; ++n) {
				a[n] /= o;
			}
		};


		// 乗算
		Complex operator*(const Complex& o) const {
			if constexpr (D == 2) {
				Complex r;
				r.a[0] = a[0] * o.a[0] - a[1] * o.a[1];
				r.a[1] = a[0] * o.a[1] + a[1] * o.a[0];
				return r;
			} else if constexpr (D == 4) {
				Complex r;
				r.a[0] = a[0] * o.a[0] - a[1] * o.a[1] - a[2] * o.a[2] -
						 a[3] * o.a[3];
				r.a[1] = a[1] * o.a[0] + a[0] * o.a[1] + a[2] * o.a[3] -
						 a[3] * o.a[2];
				r.a[2] = a[2] * o.a[0] + a[0] * o.a[2] + a[3] * o.a[1] -
						 a[1] * o.a[3];
				r.a[3] = a[3] * o.a[0] + a[0] * o.a[3] + a[1] * o.a[2] -
						 a[2] * o.a[1];
				return r;
			} else {
				static_assert(
					fail<T>,
					"複素数の乗算は2, 4要素のみ(8, 15要素は未実装)");
			}
		};


		// 回転ほか
		Complex(const ::tb::Vector<D - 1, T>& o) { // ベクタからの生成
			a[0] = 0;
			for (unsigned n(1); n < D; ++n) {
				a[n] = o[n - 1];
			}
		};
		operator ::tb::Vector<D - 1, T>() { // ベクタの生成
			return ::tb::Vector<D - 1, T>(a, 1);
		};
		Complex operator~() const { // 共役を返す
			Complex r;
			r.a[0] = a[0];
			for (unsigned n(1); n < D; ++n) {
				r.a[n] = -a[n];
			}
			return r;
		};
		::tb::Vector<D - 1, T> Rotate(const ::tb::Vector<D - 1, T>& v) const {
			auto r(::tb::Vector<D - 1, T>(*this * Complex(v) * ~*this));
			r.Normalize();
			return r;
		};
		::tb::Vector<D - 1, T>
		ReverseRotate(const ::tb::Vector<D - 1, T>& v) const {
			auto r(::tb::Vector<D - 1, T>(~*this * Complex(v) * *this));
			r.Normalize();
			return r;
		};

	private:
		template <typename U> static constexpr bool fail = false;
		T a[D];
	};
}
