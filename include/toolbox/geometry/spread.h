/************************************************************ toolbox geometry
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
#pragma once



namespace TB {

	template <unsigned D, typename T> class Spread {
	public:
		operator T*() { return value; };
		operator const T*() const { return value; }
		Spread(const T* o) {
			for (unsigned n(0); n < D; ++n) {
				value[n] = o[n];
			}
		};

		void operator*=(T t) {
			for (auto& e : value) {
				e *= t;
			}
		};
		Spread operator*(T t) const {
			Spread r(*this);
			r *= t;
			return r;
		};
		void operator/=(T t) {
			for (auto& e : value) {
				e /= t;
			}
		};
		Spread operator/(T t) const {
			Spread r(*this);
			r /= t;
			return r;
		};
		operator bool() {
			for (auto& e : value) {
				if (e == 0) {
					return false;
				}
			}
			return true;
		};

	private:
		T value[D];
	};
}
