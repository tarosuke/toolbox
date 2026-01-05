/************************************************************ toolbox geometry
 * Copyright (C) 2021, 2024 tarosuke<webmaster@tarosuke.net>
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

#include <tb/geometry/vector.h>



namespace tb::geometry {

	template <unsigned D, typename T> struct Spread {
		// NOTE:Rect他で区別できなくなるのでVectorから継承してはいけない
		Spread() : a{0, 0} {};
		Spread(const Vector<D, T>& o) { *this = o; };
		template <typename... A> Spread(A... a) : a{a...} {};

		Spread& operator=(const Spread& o) {
			for (unsigned n(0); n < D; ++n) { a[n] = o[n]; }
			return *this;
		};
		Spread& operator=(const Vector<D, T>& o) {
			for (unsigned n(0); n < D; ++n) { a[n] = o[n]; }
			return *this;
		};

		// 配列としてアクセス
		T& operator[](unsigned n) { return a[n]; };
		const T& operator[](unsigned n) const { return a[n]; };

		operator Vector<D, T>() { return Vector<D, T>(a); };
		operator const Vector<D, T>() const { return Vector<D, T>(a); };

		// 演算
		void operator*=(T t) {
			for (auto& e : a) { e *= t; }
		};
		template <typename U> Spread<D, U> operator*(U t) const {
			Spread<D, U> r;
			for (unsigned n(0); n < D; ++n) { r[n] = t * a[n]; }
			return r;
		};
		void operator/=(T t) {
			for (auto& e : a) { e /= t; }
		};
		template <typename U> Spread<D, U> operator/(U t) const {
			Spread<D, U> r;
			for (unsigned n(0); n < D; ++n) { r[n] = (U)a[n] / t; }
			return r;
		};

		bool operator==(const Spread& o) const {
			for (unsigned n(0); n < D; ++n) {
				if (a[n] != o.a[n]) {
					return false;
				}
			}
			return true;
		};

		Spread operator+(const Spread& s) const {
			Spread ss;
			for (unsigned n(0); n < D; ++n) { ss.a[n] = a[n] + s.a[n]; }
			return ss;
		};
		const Spread& operator+=(const Spread& s) {
			for (unsigned n(0); n < D; ++n) { a[n] += s.a[n]; }
			return *this;
		};

		// 面積、体積
		T Volume() const {
			T v(1);
			for (unsigned n(0); n < D; ++n) { v *= a[n]; }
		};
		bool IsEmpty() const {
			for (auto& e : a) {
				if (e <= 0) {
					return true;
				}
			}
			return false;
		};
		bool operator!() const { return IsEmpty(); };

	private:
		T a[D];
	};
}
