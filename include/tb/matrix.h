/* Rect
 * Copyright (C) 2014,2021,2023 tarosuke<webmaster@tarosuke.net>
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

	template <unsigned COL, unsigned ROW, typename T, bool ER = false>
	class Matrix {
	public:
		Matrix() = default;

		// 配列として扱う
		operator T*() { return raw; };
		operator const T*() const { return raw; };
		T* operator[](unsigned r) { return m[r]; };
		const T* operator[](unsigned r) const { return m[r]; };

		// 代入とコピーコンストラクタ
		template <typename U> const Matrix& operator=(const U (&o)[ROW][COL]) {
			for (unsigned r(0); r < ROW; ++r) {
				for (unsigned c(0); c < COL; ++c) {
					m[r][c] = (T)o[r][c];
				}
			}
			return *this;
		};
		template <typename U> const Matrix& operator=(const U (&o)[COL * ROW]) {
			for (unsigned n(0); n < COL * ROW; ++n) {
				raw[n] = (T)o[n];
			}
			return *this;
		};
		template <typename U> Matrix(const U (&o)[ROW * COL]) : Matrix() {
			*this = o;
		};
		template <typename U> Matrix(const U (&o)[ROW][COL]) : Matrix() {
			*this = o;
		};

		// その他の演算
		bool operator==(const Matrix& o) const {
			for (unsigned r(0); r < ROW; ++r) {
				for (unsigned c(0); c < COL; ++c) {
					if constexpr (ER && std::is_floating_point<T>::value) {
						if (0.000001 < abs(m[r][c] - o.m[r][c])) {
							return false;
						}
					} else {
						if (m[r][c] != o.m[r][c]) {
							return false;
						}
					}
				}
			}
			return true;
		};

		void Identity() {
			for (unsigned n(0); n < ROW * COL; ++n) {
				raw[n] = 0;
			}
			for (unsigned n(0); n < ROW && n < COL; ++n) {
				m[n][n] = 1;
			}
		};
		void Transpose(const T (&o)[COL][ROW]) {
			T* d(raw);
			for (unsigned x(0); x < COL; ++x) {
				for (unsigned y(0); y < ROW; ++y) {
					*d++ = o[y][x];
				}
			}
		};
		void TransposeAffine(const T (&o)[COL - 1][ROW]) {
			T* d(raw);
			for (unsigned x(0); x < ROW; ++x) {
				for (unsigned y(0); y < COL - 1; ++y) {
					*d++ = o[y][x];
				}
				*d++ = 0;
			}
			raw[COL * ROW - 1] = 1;
		};

		void Invert() {
			const float r(
				1.0 / (m[0][3] * m[1][2] * m[2][1] * m[3][0] -
					   m[0][2] * m[1][3] * m[2][1] * m[3][0] -
					   m[0][3] * m[1][1] * m[2][2] * m[3][0] +
					   m[0][1] * m[1][3] * m[2][2] * m[3][0] +
					   m[0][2] * m[1][1] * m[2][3] * m[3][0] -
					   m[0][1] * m[1][2] * m[2][3] * m[3][0] -
					   m[0][3] * m[1][2] * m[2][0] * m[3][1] +
					   m[0][2] * m[1][3] * m[2][0] * m[3][1] +
					   m[0][3] * m[1][0] * m[2][2] * m[3][1] -
					   m[0][0] * m[1][3] * m[2][2] * m[3][1] -
					   m[0][2] * m[1][0] * m[2][3] * m[3][1] +
					   m[0][0] * m[1][2] * m[2][3] * m[3][1] +
					   m[0][3] * m[1][1] * m[2][0] * m[3][2] -
					   m[0][1] * m[1][3] * m[2][0] * m[3][2] -
					   m[0][3] * m[1][0] * m[2][1] * m[3][2] +
					   m[0][0] * m[1][3] * m[2][1] * m[3][2] +
					   m[0][1] * m[1][0] * m[2][3] * m[3][2] -
					   m[0][0] * m[1][1] * m[2][3] * m[3][2] -
					   m[0][2] * m[1][1] * m[2][0] * m[3][3] +
					   m[0][1] * m[1][2] * m[2][0] * m[3][3] +
					   m[0][2] * m[1][0] * m[2][1] * m[3][3] -
					   m[0][0] * m[1][2] * m[2][1] * m[3][3] -
					   m[0][1] * m[1][0] * m[2][2] * m[3][3] +
					   m[0][0] * m[1][1] * m[2][2] * m[3][3]));

			const float n[16] = {
				r * (m[1][2] * m[2][3] * m[3][1] - m[1][3] * m[2][2] * m[3][1] +
					 m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] -
					 m[1][2] * m[2][1] * m[3][3] + m[1][1] * m[2][2] * m[3][3]),
				r * (m[0][3] * m[2][2] * m[3][1] - m[0][2] * m[2][3] * m[3][1] -
					 m[0][3] * m[2][1] * m[3][2] + m[0][1] * m[2][3] * m[3][2] +
					 m[0][2] * m[2][1] * m[3][3] - m[0][1] * m[2][2] * m[3][3]),
				r * (m[0][2] * m[1][3] * m[3][1] - m[0][3] * m[1][2] * m[3][1] +
					 m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] -
					 m[0][2] * m[1][1] * m[3][3] + m[0][1] * m[1][2] * m[3][3]),
				r * (m[0][3] * m[1][2] * m[2][1] - m[0][2] * m[1][3] * m[2][1] -
					 m[0][3] * m[1][1] * m[2][2] + m[0][1] * m[1][3] * m[2][2] +
					 m[0][2] * m[1][1] * m[2][3] - m[0][1] * m[1][2] * m[2][3]),
				r * (m[1][3] * m[2][2] * m[3][0] - m[1][2] * m[2][3] * m[3][0] -
					 m[1][3] * m[2][0] * m[3][2] + m[1][0] * m[2][3] * m[3][2] +
					 m[1][2] * m[2][0] * m[3][3] - m[1][0] * m[2][2] * m[3][3]),
				r * (m[0][2] * m[2][3] * m[3][0] - m[0][3] * m[2][2] * m[3][0] +
					 m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] -
					 m[0][2] * m[2][0] * m[3][3] + m[0][0] * m[2][2] * m[3][3]),
				r * (m[0][3] * m[1][2] * m[3][0] - m[0][2] * m[1][3] * m[3][0] -
					 m[0][3] * m[1][0] * m[3][2] + m[0][0] * m[1][3] * m[3][2] +
					 m[0][2] * m[1][0] * m[3][3] - m[0][0] * m[1][2] * m[3][3]),
				r * (m[0][2] * m[1][3] * m[2][0] - m[0][3] * m[1][2] * m[2][0] +
					 m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] -
					 m[0][2] * m[1][0] * m[2][3] + m[0][0] * m[1][2] * m[2][3]),
				r * (m[1][1] * m[2][3] * m[3][0] - m[1][3] * m[2][1] * m[3][0] +
					 m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] -
					 m[1][1] * m[2][0] * m[3][3] + m[1][0] * m[2][1] * m[3][3]),
				r * (m[0][3] * m[2][1] * m[3][0] - m[0][1] * m[2][3] * m[3][0] -
					 m[0][3] * m[2][0] * m[3][1] + m[0][0] * m[2][3] * m[3][1] +
					 m[0][1] * m[2][0] * m[3][3] - m[0][0] * m[2][1] * m[3][3]),
				r * (m[0][1] * m[1][3] * m[3][0] - m[0][3] * m[1][1] * m[3][0] +
					 m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] -
					 m[0][1] * m[1][0] * m[3][3] + m[0][0] * m[1][1] * m[3][3]),
				r * (m[0][3] * m[1][1] * m[2][0] - m[0][1] * m[1][3] * m[2][0] -
					 m[0][3] * m[1][0] * m[2][1] + m[0][0] * m[1][3] * m[2][1] +
					 m[0][1] * m[1][0] * m[2][3] - m[0][0] * m[1][1] * m[2][3]),
				r * (m[1][2] * m[2][1] * m[3][0] - m[1][1] * m[2][2] * m[3][0] -
					 m[1][2] * m[2][0] * m[3][1] + m[1][0] * m[2][2] * m[3][1] +
					 m[1][1] * m[2][0] * m[3][2] - m[1][0] * m[2][1] * m[3][2]),
				r * (m[0][1] * m[2][2] * m[3][0] - m[0][2] * m[2][1] * m[3][0] +
					 m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] -
					 m[0][1] * m[2][0] * m[3][2] + m[0][0] * m[2][1] * m[3][2]),
				r * (m[0][2] * m[1][1] * m[3][0] - m[0][1] * m[1][2] * m[3][0] -
					 m[0][2] * m[1][0] * m[3][1] + m[0][0] * m[1][2] * m[3][1] +
					 m[0][1] * m[1][0] * m[3][2] - m[0][0] * m[1][1] * m[3][2]),
				r * (m[0][1] * m[1][2] * m[2][0] - m[0][2] * m[1][1] * m[2][0] +
					 m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] -
					 m[0][1] * m[1][0] * m[2][2] +
					 m[0][0] * m[1][1] * m[2][2])};

			*this = n;
		};
		void InvertAffine() { Invert(); };

		// ROW行COL列×COL行C列 = ROW行C列
		template <unsigned C>
		Matrix<C, ROW, T> operator*(const Matrix<C, COL, T>& o) const {
			Matrix<C, ROW, T> rv;

			for (unsigned r(0); r < ROW; ++r) {
				for (unsigned c(0); c < C; ++c) {
					auto& t(rv[r][c]);
					t = 0;
					for (unsigned n(0); n < COL; ++n) {
						t += m[r][n] * o[n][c];
					}
				}
			}

			return rv;
		};

		// Matrix<ROW, COL, T> * Vector<ROW - 1, T> → Vector<ROW - 1, T>
		// NOTE:Vector as Matrix(ROW, 1, T>
		Vector<ROW - 1, T> operator*(const Vector<COL - 1, T>& v) const {
			Vector<ROW - 1, T> r;
			for (unsigned n(0); n < ROW - 1; ++n) {
				for (unsigned c(0); c < COL - 1; ++c) {
					r[n] += v[c] * m[n][c];
				}
				r[n] += m[n][COL - 1];
			}
			return r;
		};

	private:
		union {
			T raw[COL * ROW];
			T m[ROW][COL];
		};
	};
}
