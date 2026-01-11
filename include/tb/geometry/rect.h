/* Rect
 * Copyright (C) 2016, 2021, 2024, 2025, 2026 tarosuke<webmaster@tarosuke.net>
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

#include <limits>
#include <tb/geometry/line.h>
#include <tb/geometry/spread.h>
#include <tb/geometry/vector.h>



namespace tb::geometry {

	template <unsigned D, typename T> struct Rect {
		Rect() {};
		template <typename U, typename V>
		Rect(const Vector<D, U>& a, const Vector<D, V>& b) {
			for (unsigned n(0); n < D; ++n) {
				if (a[n] < b[n]) {
					origin[n] = a[n];
					spread[n] = b[n] - a[n];
				} else {
					origin[n] = b[n];
					spread[n] = a[n] - b[n];
				}
			}
		};
		template <typename U, typename V>
		Rect(const Vector<D, U>& o, const tb::geometry::Spread<D, V>& s) :
			origin(o),
			spread(s){};
		template <typename U>
		Rect(const tb::geometry::Spread<D, U>& s) : spread(s){};

		// 比較
		template <typename U> bool operator==(const Rect<D, U>& t) const {
			return origin == t.origin && spread == t.spread;
		};

		// 移動
		template <typename U> Rect& operator+=(const Vector<D, U>& t) {
			origin += t;
			return *this;
		};
		template <typename U> Rect operator+(const Vector<D, U>& t) const {
			Rect r(*this);
			return r += t;
		};
		template <typename U> Rect& operator-=(const Vector<D, U>& t) {
			origin -= t;
			return *this;
		};
		template <typename U> Rect operator-(const Vector<D, U>& t) const {
			Rect r(*this);
			return r -= t;
		};

		// 拡大縮小
		template <typename U> Rect& operator+=(const Spread<D, U>& t) {
			spread += t;
			return *this;
		};
		template <typename U> Rect& operator-=(const Spread<D, U>& t) {
			spread -= t;
			return *this;
		};
		template <typename U> Rect& operator*=(const U& t) {
			spread *= t;
			return *this;
		};
		template <typename U> Rect& operator/=(const U& t) {
			spread /= t;
			return *this;
		};

		// 重なり
		template <typename U> Rect& operator|=(const Rect<D, U>& t) {
			for (unsigned n(0); n < D; ++n) {
				const T s(origin[n] < t.origin[n] ? origin[n] : t.origin[n]);
				const T e(origin[n] + spread[n] < t.origin[n] + t.spread[n]
							  ? t.origin[n] + t.spread[n]
							  : origin[n] + spread[n]);
				origin[n] = s;
				spread[n] = e - s;
			}
			return *this;
		};
		template <typename U> Rect operator|(const Rect<D, U>& t) const {
			Rect r(*this);
			r |= t;
			return r;
		};
		template <typename U> Rect& operator&=(const Rect<D, U>& t) {
			for (unsigned n(0); n < D; ++n) {
				const T a(origin[n] < t.origin[n] ? t.origin[n] : origin[n]);
				const T r0(origin[n] + spread[n]);
				const T r1(t.origin[n] + t.spread[n]);
				const T b(r0 < r1 ? r0 : r1);
				origin[n] = a;
				spread[n] = b - a;
			}
			return *this;
		};
		template <typename U> Rect operator&(const Rect<D, U>& t) const {
			Rect r(*this);
			r &= t;
			return r;
		};

		// 範囲判定
		template <typename U> bool operator&&(const Vector<D, U>& t) {
			for (unsigned n(0); n < D; ++n) {
				if (t[n] < origin[n] || origin[n] + spread[n] < t[n]) {
					return false;
				}
			}
			return true;
		};
		template <typename U> bool operator&&(const Rect<D, U>& t) {
			for (unsigned n(0); n < D; ++n) {
				if (t.origin[n] + t.spread[n] <= origin[n] ||
					origin[n] + spread[n] <= t.origin[n]) {
					return false;
				}
			}
			return true;
		};
		template <typename U> bool operator&&(const Line<D, U>& t) {
			T min(std::numeric_limits<T>::infinity());
			T max(-std::numeric_limits<T>::infinity());
			for (unsigned n(0); n < D; ++n) {
				// 軸nについて
				if (t.Direction()[n] != 0.0) {
					// 座標軸と平行ではない軸について媒介変数を取得
					const T p0(t.Parameter(n, origin[n]));
					const T p1(t.Parameter(n, origin[n] + spread[n]));
					if (p0 < p1) {
						if (min < p1) {
							min = p1;
						}
						if (p0 < max) {
							max = p0;
						}
					} else {
						if (min < p0) {
							min = p0;
						}
						if (p1 < max) {
							max = p1;
						}
					}
				}
			}
			return min <= max;
		};



		// 型変換、プロパティ
		bool operator!() { return !spread; };
		const Vector<D, T>& Origin() const { return origin; };
		T Origin(unsigned d) const { return origin[d]; };
		template <typename U> void Origin(const Vector<D, U>& t) {
			origin = t;
		};
		void Origin(unsigned d, const T& v) { origin[d] = v; };
		const tb::geometry::Spread<D, T>& Spread() const { return spread; };
		T Spread(unsigned d) const { return spread[d]; };
		template <typename U> void Spread(const tb::geometry::Spread<D, U>& t) {
			spread = t;
		};
		void Spread(unsigned d, const T& v) { spread[d] = v; };
		const Vector<D, T>& Left() const { return origin; };
		Vector<D, T> Right() const { return origin + spread; };

	private:
		Vector<D, T> origin;			   // 与えた値の小さい方になる
		tb::geometry::Spread<D, T> spread; // 負の値があったら空
	};

}
