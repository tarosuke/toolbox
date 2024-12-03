/** Vector
 * Copyright (C) 2016, 2017, 2021, 2023, 2024 tarosuke<webmaster@tarosuke.net>
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

#include <tb/types.h>
#include <type_traits>

namespace tb {
	template <uint D, typename T> struct Vector {
		///// 構築子、代入、[]
		Vector() { Clear(); };
		void Clear() {
			for (uint n(0); n < D; ++n) {
				a[n] = 0;
			}
		};
		Vector(const Vector&) = default;
		template <typename... A> Vector(T t, A... a) : a{t, a...} {};
		Vector(T o) {
			for (uint n(0); n < D; ++n) {
				*const_cast<T*>(&a[n]) = o;
			}
		};
		Vector(const T (&o)[D]) {
			for (unsigned n(0); n < D; ++n) {
				a[n] = o[n];
			}
		};
		Vector(const T (&o)[D + 1], unsigned offset) {
			for (uint n(0); n < D; ++n) {
				a[n] = o[n + offset];
			}
		};

		operator auto &() { return a; };
		operator const auto &() const { return a; };
		template <typename U> const Vector& operator=(const U& o) {
			for (unsigned n(0); n < D; ++n) {
				a[n] = o[n];
			}
			return *this;
		}

		// 比較(実数の場合誤差が一定以下なら同値とする)
		bool operator==(const Vector& o) const {
			for (uint n(0); n < D; ++n) {
				if (a[n] != o[n]) {
					return false;
				}
			}
			return true;
		};

		///// 四則演算
		template <typename U> Vector operator+(const U& o) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.a[n] = a[n] + o[n];
			}
			return r;
		};
		template <typename U> Vector operator-(const U& o) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.a[n] = a[n] - o[n];
			}
			return r;
		};
		template <typename U> Vector operator*(U o) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.a[n] = a[n] * o;
			}
			return r;
		};
		template <typename U> Vector operator/(U o) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.a[n] = a[n] / o;
			}
			return r;
		}

		template <typename U> const Vector& operator+=(const U& o) {

			for (uint n(0); n < D; ++n) {
				a[n] += o[n];
			}
			return *this;
		};
		template <typename U> const Vector& operator-=(const U& o) {
			for (uint n(0); n < D; ++n) {
				a[n] -= o[n];
			}
			return *this;
		};
		template <typename U> const Vector& operator*=(U o) {
			for (uint n(0); n < D; ++n) {
				a[n] *= o;
			}
			return *this;
		};
		template <typename U> const Vector& operator/=(U o) {
			for (uint n(0); n < D; ++n) {
				a[n] /= o;
			}
			return *this;
		};

		///// ノルム
		T Norm2() const {
			T r(0);
			for (uint n(0); n < D; ++n) {
				r += a[n] * a[n];
			}
			return r;
		};
		T Norm() const { return sqrt(Norm2()); };
		void Normalize() { *this / Norm(); };

		///// 積
		T operator*(const Vector& o) const {
			T r(0);
			for (uint n(0); n < D; ++n) {
				r += a[n] * o.a[n];
			}
			return r;
		};

		Vector Cross(const Vector& o) const {
			if constexpr (D == 3) {
				return Vector<3, T>(a[1] * o.a[2] - a[2] * o.a[1],
					a[2] * o.a[0] - a[0] * o.a[2],
					a[0] * o.a[1] - a[1] * o.a[0]);
			} else if constexpr (D == 7) {
				static_assert(false, "その次元数のクロス積は未実装");
			} else {
				static_assert(false, "その次元数のクロス積は定義できない");
			}
		};

	protected:
		T a[D];
	};

	template <typename T>
	T Cross2(const Vector<2, T>& a, const Vector<2, T>& b) {
		return a[0] * b[1] - a[1] * b[0];
	}
}
