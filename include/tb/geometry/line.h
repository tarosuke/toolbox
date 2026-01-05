/* Copyright (C) 2025 tarosuke<webmaster@tarosuke.net>
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

	template <unsigned D, typename T> struct Line {
		template <typename U>
		Line(const Vector<D, U>& origin, const Vector<D, U>& direction) :
			origin(origin),
			direction(direction){};

		template <typename U> Line& operator+=(const Vector<D, U>& t) {
			origin += t;
			return *this;
		};

		T Parameter(const unsigned axis, const T& value) {
			// 軸axisの値がvalueのときの媒介変数を返す
			// Note:Direction()[axis]が0のときの挙動は未定義
			return (value - origin[axis]) / direction[axis];
		};
		const Vector<D, T>& Origin() { return origin; }
		const Vector<D, T>& Direction() { return direction; }

	private:
		Vector<D, T> origin;
		Vector<D, T> direction;
	};
}
