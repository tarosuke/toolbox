#include <tb/matrix.h>
#include <tb/string.h>
#include <tb/test.h>



using M22 = tb::Matrix<2, 2, int>;

template <unsigned R, unsigned C>
tb::String Show(const tb::Matrix<C, R, int>& v) {
	tb::String s("{ ");
	for (unsigned r(0); r < R; ++r) {
		s += "{ ";
		for (unsigned c(0); c < C; ++c) {
			s.Append(v[r][c]);
			s += ", ";
		}
		s += "}";
	}
	s += "}";
	return s;
}

template <unsigned N> tb::String Show(const tb::geometry::Vector<N, int>& v) {
	tb::String s("{ ");
	for (unsigned n(0); n < N; ++n) {
		s.Append(v[n]);
		s += ", ";
	}
	s += "}";
	return s;
}

template <> tb::String::String(const M22& v) : String(Show(v)){};
template <>
tb::String::String(const tb::Matrix<2, 3, int>& v) : String(Show(v)){};
template <>
tb::String::String(const tb::Matrix<4, 2, int>& v) : String(Show(v)){};
template <>
tb::String::String(const tb::Matrix<4, 3, int>& v) : String(Show(v)){};
template <>
tb::String::String(const tb::Matrix<3, 3, int>& v) : String(Show(v)){};
template <>
tb::String::String(const tb::geometry::Vector<2, int>& v) : String(Show(v)){};


int main() {
	{
		const M22 a((const int[4]){1, 2, 3, 4});
		const M22 b((const int[4]){5, 6, 7, 8});
		const M22 c((const int[4]){19, 22, 43, 50});
		assertEQ(a * b, c);
	}

	{
		const tb::Matrix<2, 3, int> a((const int[6]){0, 1, 2, 3, 4, 5});
		const tb::Matrix<4, 2, int> b((const int[8]){0, 1, 2, 3, 4, 5, 6, 7});
		const tb::Matrix<4, 3, int> c(
			(const int[]){4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47});
		assertEQ(a * b, c);
	}

	{
		const tb::Matrix<3, 3, int> a((const int[9]){1, 2, 3, 4, 5, 6});
		const tb::geometry::Vector<2, int> b({10, 11});
		const tb::geometry::Vector<2, int> c({35, 101});
		assertEQ(a * b, c);
	}

	return 0;
}
