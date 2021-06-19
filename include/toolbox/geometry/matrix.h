#pragma once

#include <toolbox/geometry/vector.h>



namespace TB{

	template<
		unsigned COL,
		unsigned ROW,
		typename T = float> class Matrix{
	public:
		Matrix(){};

		// 配列として扱う
		operator T*(){ return raw; };
		operator T const*() const { return raw; };
		T* operator[](unsigned r){ return m[r]; };
		const T* operator[](unsigned r) const { return m[r]; };

		// 代入とコピーコンストラクタ
		template <typename U> void operator=(const U (&o)[ROW][COL]) {
			for (unsigned r(0); 0 < ROW; ++r) {
				for (unsigned c(0); c < COL; ++c) {
					m[r][c] = (T)o[r][c];
				}
			}
		};
		template <typename U> void operator=(const U (&o)[COL * ROW]) {
			for (unsigned n(0); n < COL * ROW; ++n) {
				raw[n] = (T)o[n];
			}
		};
		template <typename U> Matrix(const U (&o)[ROW * COL]) { *this = o; };
		template <typename U> Matrix(const U (&o)[ROW][COL]) { *this = o; };

		// その他の演算
		bool operator==(const Matrix& o) const {
			for (unsigned r(0); r < ROW; ++r) {
				for (unsigned c(0); c < COL; ++c) {
					if (m[r][c] != o.m[r][c]) {
						return false;
					}
				}
			}
			return true;
		};
		bool operator!=(const Matrix& o) { return !(*this == o); };

		void Identity(){
			for(unsigned n(0); n < ROW * COL; ++n){
				raw[n] = 0;
			}
			for(unsigned n(0); n < ROW && n < COL; ++n){
				m[n][n] = 1;
			}
		};
		void Transpose(const T o[COL][ROW]){
			T* d(raw);
			for(unsigned x(0); x < COL; ++x){
				for(unsigned y(0); y < ROW; ++y){
					*d++ = o[y][x];
				}
			}
		};
		void TransposeAffine(const T o[COL - 1][ROW]) {
			T* d(raw);
			for(unsigned x(0); x < ROW; ++x){
				for(unsigned y(0); y < COL - 1; ++y){
					*d++ = o[y][x];
				}
				*d++ = 0;
			}
			raw[COL * ROW - 1] = 1;
		};

		void Invert();
		void InvertAffine(){ Invert(); };

		// ROW行COL列×COL行C列 = ROW行C列
		template <unsigned C>
		Matrix<C, ROW, T> operator*(const Matrix<C, COL, T>& o) const {
			Matrix<C, ROW, T> rv;

			for(unsigned r(0); r < ROW; ++r){
				for(unsigned c(0); c < C; ++c){
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
		union{
			T raw[COL * ROW];
			T m[ROW][COL];
		};
	};
}
