#include <toolbox/test.h>

#include <toolbox/geometry/matrix.h>



using M22 = TB::Matrix<2, 2, int>;

template <unsigned R, unsigned C>
TB::String Show(const TB::Matrix<C, R, int>& v) {
	TB::String s("{ ");
	for (unsigned r(0); r < R; ++r) {
		s << "{ ";
		for (unsigned c(0); c < C; ++c) {
			s << v[r][c] << ", ";
		}
		s << "}";
	}
	s << "}";
	return s;
}

template <unsigned N> TB::String Show(const TB::Vector<N, int>& v) {
	TB::String s("{ ");
	for (unsigned n(0); n < N; ++n) {
		s << v[n] << ", ";
	}
	s << "}";
	return s;
}

template <> TB::String::String(const M22& v) : String(Show(v)){};
template <> TB::String::String(const TB::Matrix<2, 3, int>& v)
	: String(Show(v)){};
template <> TB::String::String(const TB::Matrix<4, 2, int>& v)
	: String(Show(v)){};
template <> TB::String::String(const TB::Matrix<4, 3, int>& v)
	: String(Show(v)){};
template <> TB::String::String(const TB::Matrix<3, 3, int>& v)
	: String(Show(v)){};
template <> TB::String::String(const TB::Vector<2, int>& v) : String(Show(v)){};


int main() {
	{
		const M22 a((const int[4]){1, 2, 3, 4});
		const M22 b((const int[4]){5, 6, 7, 8});
		const M22 c((const int[4]){19, 22, 43, 50});
		assertEQ(a * b, c);
	}

	{
		const TB::Matrix<2, 3, int> a((const int[6]){0, 1, 2, 3, 4, 5});
		const TB::Matrix<4, 2, int> b((const int[8]){0, 1, 2, 3, 4, 5, 6, 7});
		const TB::Matrix<4, 3, int> c(
			(const int[]){4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47});
		assertEQ(a * b, c);
	}

	{
		const TB::Matrix<3, 3, int> a((const int[9]){1, 2, 3, 4, 5, 6});
		const TB::Vector<2, int> b((const int[2]){10, 11});
		const TB::Vector<2, int> c((const int[2]){35, 101});
		assertEQ(a * b, c);
	}

	return 0;
}
