/************************************************************ toolbox geometry
 * Copyright (C) 2021, 2924 tarosuke<webmaster@tarosuke.net>
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

#include <tb/vector.h>



namespace tb {

	template <unsigned D, typename T> struct Spread {
		// NOTE:Rect他で区別できなくなるのでVectorから継承してはいけない
		Spread() : a{0, 0} {};
		Spread(const Vector<D, T>& o) : a(o){};
		template <typename... A> Spread(T t, A... a) : a{t, a...} {};

		Spread& operator=(const Spread& o) {
			for (unsigned n(0); n < D; ++n) {
				a[n] = o[n];
			}
		};
		Spread& operator=(const Vector<D, T>& o) {
			for (unsigned n(0); n < D; ++n) {
				a[n] = o[n];
			}
		};

		// 配列として取り出す
		operator auto &() { return a; };
		operator const auto &() { return a; };

		// 演算
		void operator*=(T t) {
			for (auto& e : a) {
				e *= t;
			}
		};
		Spread operator*(T t) const {
			Spread r(*this);
			r *= t;
			return r;
		};
		void operator/=(T t) {
			for (auto& e : a) {
				e /= t;
			}
		};
		Spread operator/(T t) const {
			Spread r(*this);
			r /= t;
			return r;
		};
		operator bool() {
			for (auto& e : a) {
				if (e <= 0) {
					return false;
				}
			}
			return true;
		};

	private:
		T a[D];
	};
}
