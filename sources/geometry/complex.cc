/** Complexes
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
#include <toolbox/geometry/complex.h>

#include <math.h>



namespace {

	// ベクトルfrom→toの四元数を求める
	template <typename T> void
	Diff(T (&r)[4], const TB::Vector<3, T>& from, const TB::Vector<3, T>& to) {
		TB::Vector<3, T> ff(from);
		TB::Vector<3, T> tt(to);
		ff.Normalize();
		tt.Normalize();

		const T in(tt * ff); //内積(差分の回転角)
		TB::Vector<3, T> ex(tt.Cross(ff)); //外積(差分の回転軸)

		const T c(sqrt(0.5 * (1 + in)));
		const T s(sqrt(0.5 * (1 - in)));

		r[0] = c; //実部はc
		for (unsigned n(1); n < 4; ++n) {
			r[n] = s * ex[n];
		}
	}

}

namespace TB {

	template <> Complex<4, float>::Complex(
		const TB::Vector<3, float>& from, const TB::Vector<3, float>& to) {
		Diff(value, from, to);
		Normalize();
	}
	template <> Complex<4, double>::Complex(
		const TB::Vector<3, double>& from, const TB::Vector<3, double>& to) {
		Diff(value, from, to);
		Normalize();
	}

}
