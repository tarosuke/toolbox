/* Rect
 * Copyright (C) 2016, 2021, 2024 tarosuke<webmaster@tarosuke.net>
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

#include <tb/spread.h>
#include <tb/vector.h>



namespace tb {

	template <unsigned D, typename T> struct Rect {
		Rect(){};
		Rect(const Vector<D, T>& a, const Vector<D, T>& b)
			: left(Less(a, b)), right(More(a, b)){};
		Rect(const Vector<D, T>& p, const Spread<D, T>& s) : Rect(p, p + s){};
		Rect(const Spread<D, T>& s) {
			for (unsigned n(0); n < D; ++n) {
				left[n] = 0;
				right[n] = s[n];
			}
		};

		Rect& operator=(const Rect&) = default;
		template <typename U> const Rect& operator=(const Rect<D, U>& o) {
			left = (T)o.left;
			right = (T)o.right;
		};


		void Clear() {
			left.Clear();
			right.Clear();
		};
		Rect& operator|=(const Rect& t) {
			if (!t) {
				// tが空なので何もしない
				return *this;
			} else if (!*this) {
				// *thisが空なのでtに置き換える
				*this = t;
				return *this;
			}
			// cどちらも空ではないので合成
			const Vector<D, T> lp(Less(left, t.left));
			const Vector<D, T> mp(More(right, t.right));
			left = lp;
			right = mp;
			return *this;
		};
		Rect operator|(const Rect& t) {
			if (!t) {
				// tが空なので*thisのコピー
				return *this;
			} else if (!*this) {
				// *thisが空なのでtのコピー
				return t;
			}
			// cどちらも空ではないので合成
			return Rect(Less(*this, t), More(*this, t));
		};
		Rect& operator&=(const Rect& t) {
			if (!t || !*this) {
				// どちらかが空
				Clear();
				return *this;
			}
			left = More(left, t.left);
			right = Less(right, t.right);
			return *this;
		};
		Rect operator&(const Rect& t) const {
			if (!t || !*this) {
				// どちらかが空
				return Rect();
			}
			return Rect(More(left, t.left), Less(right, t.right));
		};

		bool operator&&(const Vector<D, T>& o) const {
			return !!this && left[0] <= o[0] && left[1] <= o[1] &&
				   o[0] < right[0] && o[1] < right[1];
		};
		bool operator&&(const Rect<D, T>& o) const { return *this & o; };

		const Vector<D, T>& Left() const { return left; };
		const Vector<D, T>& Right() const { return right; };
		Rect operator+(const Vector<D, T>& t) const {
			return Rect(left + t, right + t);
		};
		Rect operator-(const Vector<D, T>& t) const {
			return Rect(left - t, right - t);
		};
		Rect& operator+=(const Vector<D, T>& t) {
			left += t;
			right += t;
			return *this;
		};
		Rect& operator-=(const Vector<D, T>& t) {
			left -= t;
			right -= t;
			return *this;
		};
		Rect& operator*=(T m) {
			left *= m;
			right *= m;
			return *this;
		};
		Rect operator*(T m) { return Rect{left * m, right * m}; };
		operator bool() const {
			for (unsigned n(0); n < D; ++n) {
				if (right[n] <= left[n]) {
					return false;
				}
			}
			return true;
		};
		operator Vector<D, T>&() { return left; };
		operator Spread<D, T>() { return Spread<D, T>(right - left); };

	private:
		Vector<D, T> left; // keep left lesser value
		Vector<D, T> right;
		static Vector<D, T> Less(const Vector<D, T>& a, const Vector<D, T>& b) {
			T v[D];
			for (unsigned n(0); n < D; ++n) {
				const T aa(a[n]);
				const T bb(b[n]);
				v[n] = aa < bb ? aa : bb;
			}
			return Vector<D, T>(v);
		};
		static Vector<D, T> More(const Vector<D, T>& a, const Vector<D, T>& b) {
			T v[D];
			for (unsigned n(0); n < D; ++n) {
				const T aa(a[n]);
				const T bb(b[n]);
				v[n] = aa < bb ? bb : aa;
			}
			return Vector<D, T>(v);
		};
	};
}
