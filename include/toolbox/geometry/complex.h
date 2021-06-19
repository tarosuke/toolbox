/** Complexes
 * Copyright (C) 2017, 2021 tarosuke<webmaster@tarosuke.net>
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
 *
 * dim: dimensions
 *     2: a + ib
 *     4: a + ib + jc + kd
 */
#pragma once

#include <toolbox/geometry/vector.h>



namespace TB {

	template <unsigned D, typename T> class Complex {
	public:
		// コンストラクタ
		Complex() { Normalize(); };

		// 配列に見えるアクセサ
		operator T*() { return value; };

		// 配列からの代入演算子、コンストラクタ
		template <typename U> void operator=(const U (&o)[D]) {
			for (unsigned n(0); n < D; ++n) {
				value[n] = (T)o[n];
			}
		};
		template <typename U> Complex(const U (&o)[D]) { *this = o; };

		// Complexからの代入演算子、コンストラクタ
		template <typename U> void operator=(const Complex<D, U>& o) {
			for (unsigned n(0); n < D; ++n) {
				value[n] = (T)o[n];
			}
		};
		template <typename U> Complex(const Complex<D, U>& o) { *this = o; };

		// ベクトルfrom→toの四元数を求める
		Complex(const Vector<D - 1, T>& from, const Vector<D - 1, T>& to);



		// 演算子
		void Normalize() {
			value[0] = 1;
			for (unsigned n(1); n < D; ++n) {
				value[n] = 0;
			}
		};

	private:
		T value[D];
	};
}