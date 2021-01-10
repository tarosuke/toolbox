#pragma once

#include <GL/glew.h>
#include <GL/gl.h>


namespace TB{

	template<
		unsigned COL = 4,
		unsigned ROW = 4,
		typename T = float> class Matrix{
	public:
		Matrix(){};
		Matrix(const T*);

		operator T*(){ return raw; };
		operator T const *() const { return raw; };

		void Identity(){
			for(unsigned n(0); n < ROW * COL; ++n){
				raw[n] = 0;
			}
			for(unsigned n(0); n < ROW && n < COL; ++n){
				m[n][n] = 1;
			}
		};
		void Transpose(const T o[COL][ROW]){
			float* d(raw);
			for(unsigned x(0); x < COL; ++x){
				for(unsigned y(0); y < ROW; ++y){
					*d++ = o[y][x];
				}
			}
		};
		void TransposeAffine(const T o[COL - 1][ROW]){
			float* d(raw);
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

		template<unsigned C>
			Matrix<C, ROW, T> operator *(const Matrix<C, COL, T>& o) const {
			Matrix<C, ROW, T> rv{};

			for(unsigned r(0); r < ROW; ++r){
				for(unsigned c(0); c < C; ++c){
					for(unsigned n(0); n < COL; ++n){
						rv[r][c] += m[r][n] * o[n][r];
					}
				}
			}

			return rv;
		};

	private:
		union{
			T raw[COL * ROW];
			T m[ROW][COL];
		};
	};

}
