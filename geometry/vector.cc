/** Vector
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
#include <toolbox/geometry/vector.h>



namespace {
	template <typename T> TB::Vector<3, T> Cross3(const T a[3], const T b[3]) {
		const double arr[3] = {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]};
		return TB::Vector<3, T>(arr);
	}
}

namespace TB {
	template <>
	Vector<3, float> Vector<3, float>::Cross(const Vector& t) const {
		return ::Cross3(value, t.value);
	}
	template <>
	Vector<3, double> Vector<3, double>::Cross(const Vector& t) const {
		return ::Cross3(value, t.value);
	}
}
