/** Vector
 * Copyright (C) 2016, 2017, 2021, 2023 tarosuke<webmaster@tarosuke.net>
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



namespace tb {
	template <uint D, typename T> struct Vector {
		///// 構築子、代入、[]
		Vector() = delete;
		Vector(const Vector&) = default;

		Vector(T o) {
			for (uint n(0); n < D; ++n) {
				*const_cast<T*>(&a[n]) = o;
			}
		};
		template <unsigned E> Vector(const T (&o)[E], unsigned offset = 0) {
			for (uint n(0); n < D; ++n) {
				*const_cast<T*>(&a[n]) = o[n + E - D];
			}
		};


		template <unsigned E> const Vector& operator=(const T (&o)[E]) {
			for (uint n(0); n < D; ++n) {
				a[n] = o[n];
			}
		};
		template <unsigned E> const Vector& operator=(const Vector<E, T>& o) {
			return *this = o.arr;
		};

		T operator[](unsigned n) const { return n < D ? a[n] : 0; };


		// 比較(実数の場合誤差が一定以下なら同値とする)
		bool operator==(const Vector& o) const {
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
		bool operator!=(const Vector& o) const { return !(*this == o); };


		///// 四則演算
		Vector operator+(const Vector& o) const {
			T r[D]{};
			for (uint n(0); n < D; ++n) {
				r[n] = a[n] + o.a[n];
			}
			return Vector(r);
		};
		Vector operator-(const Vector& o) const {
			T r[D];
			for (uint n(0); n < D; ++n) {
				r[n] = a[n] - o.a[n];
			}
			return Vector(r);
		};
		Vector operator*(T o) const {
			T r[D];
			for (uint n(0); n < D; ++n) {
				r[n] = a[n] * o;
			}
			return Vector(r);
		};
		Vector operator/(T o) const {
			T r[D];
			for (uint n(0); n < D; ++n) {
				r[n] = a[n] / o;
			}
			return Vector(r);
		}

		const Vector& operator+=(const Vector& o) {
			for (uint n(0); n < D; ++n) {
				a[n] += o.a[n];
			}
			return *this;
		};
		const Vector& operator-=(const Vector& o) {
			for (uint n(0); n < D; ++n) {
				a[n] -= o.a[n];
			}
			return *this;
		};
		const Vector& operator*=(T o) {
			for (uint n(0); n < D; ++n) {
				a[n] *= o;
			}
			return *this;
		};
		const Vector& operator/=(T o) {
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
			return tb::Vector<3, T>((const T[D]){
				a[1] * o.a[2] - a[2] * o.a[1],
				a[2] * o.a[0] - a[0] * o.a[2],
				a[0] * o.a[1] - a[1] * o.a[0]});
		};

	private:
		T a[D];
	};

	template <typename T>
	T Cross2(const Vector<2, T>& a, const Vector<2, T>& b) {
		return a[0] * b[1] - a[1] * b[0];
	}
}



namespace TB {

	template <uint D, typename T> class Vector {
	public:
		operator T*() { return value; };
		operator const T*() const { return value; };

		// 型違いコピー
		template <typename U> const Vector& operator=(const Vector<D, U>& t) {
			for (uint n(0); n < D; ++n) {
				value[n] = (T)t[n];
			}
			return *this;
		};
		template <typename U> const Vector& operator=(const U (&v)[D]) {
			for (uint n(0); n < D; ++n) {
				value[n] = (T)v[n];
			}
			return *this;
		};

		// 次元、型違いコピー
		template <uint E, typename U>
		const Vector& operator=(const Vector<E, U>& t) {
			for (uint n(0); n < D; ++n) {
				value[n] = n < E ? (T)t[n] : 0;
			}
			return *this;
		};
		template <uint E, typename U> const Vector& operator=(const U (&v)[E]) {
			for (uint n(0); n < D; ++n) {
				value[n] = n < E ? (T)v[n] : 0;
			}
			return *this;
		}

		// コピーコンストラクタ
		template <uint E, typename U> Vector(const Vector<E, U>& t) {
			*this = t;
		}
		template <uint E, typename U> Vector(const U (&t)[E]) { *this = t; };



		Vector() { Clear(); };
		void Clear() {
			for (uint n(0); n < D; ++n) {
				value[n] = 0;
			}
		};

		bool operator==(const Vector& t) const {
			for (uint n(0); n < D; ++n) {
				if (value[n] != t.value[n]) {
					return false;
				}
			}
			return true;
		};
		bool operator!=(const Vector& t) const { return !operator==(t); };
		Vector operator+(const Vector& t) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.value[n] = value[n] + t.value[n];
			}
			return r;
		};
		void operator+=(const Vector& t) {
			for (uint n(0); n < D; ++n) {
				value[n] += t.value[n];
			}
		};
		Vector operator-(const Vector& t) const {
			Vector r;
			for (uint n(0); n < D; ++n) {
				r.value[n] = value[n] - t.value[n];
			}
			return r;
		};
		void operator-=(const Vector& t) {
			for (uint n(0); n < D; ++n) {
				value[n] -= t.value[n];
			}
		};
		template <typename U> void operator*=(U t) {
			for (uint n(0); n < D; ++n) {
				value[n] *= t;
			}
		};
		template <typename U> Vector operator*(U t) const {
			Vector v(*this);
			v *= t;
			return v;
		};
		template <typename U> void operator/=(U t) {
			for (uint n(0); n < D; ++n) {
				value[n] /= t;
			}
		};
		template <typename U> Vector operator/(U t) const {
			Vector v(*this);
			v /= t;
			return v;
		};
		T operator*(const Vector& t) const {
			T v(0);
			for (uint n(0); n < D; ++n) {
				v += value[n] * t.value[n];
			}
			return v;
		};
		void Min(const Vector& t) {
			for (uint n(0); n < D; ++n) {
				if (t.value[n] < value[n]) {
					value[n] = t.value[n];
				}
			}
		};
		void Max(const Vector& t) {
			for (uint n(0); n < D; ++n) {
				if (value[n] < t.value[n]) {
					value[n] = t.value[n];
				}
			}
		};

		T Length2() const {
			T l2(0);
			for (const T e : value) {
				l2 += e;
			}
			return l2;
		};
		T Length() const { return sqrt(Length2()); };

		void Normalize() { *this /= Length(); };

		T Cross2(const Vector& t) const {
			return value[0] * t.value[1] - value[1] * t.value[0];
		};
		Vector Cross(const Vector&) const;

	private:
		T value[D];
	};
};
