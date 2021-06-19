#include <stdio.h>
#include <stdlib.h>

#include <toolbox/geometry/matrix.h>


using M22 = TB::Matrix<2, 2, int>;

template <unsigned R, unsigned C>
void Check(const TB::Matrix<C, R, int>& a, const TB::Matrix<C, R, int>& q) {
	if (a == q) {
		return;
	}

	puts("\n[matrix]\n");
	for (unsigned r(0); r < R; ++r) {
		for (unsigned c(0); c < C; ++c) {
			printf(
				"q[%u][%u]:%d,\ta[%u][%u]:%d\n",
				c,
				r,
				q[r][c],
				c,
				r,
				a[r][c]);
		}
	}
	exit(-1);
}

void Check(
	unsigned dim, const TB::Vector<2, int>& a, const TB::Vector<2, int> q) {
	if (a == q) {
		return;
	}

	puts("\n[matrix]\n");
	for (unsigned n(0); n < dim; ++n) {
		printf("q[%u]:%3d,\ta[%u]:%3d\n", n, q[n], n, a[n]);
	}
	exit(-1);
}

int main() {
	{
		const M22 a((const int[4]){1, 2, 3, 4});
		const M22 b((const int[4]){5, 6, 7, 8});
		const M22 c((const int[4]){19, 22, 43, 50});
		Check(a * b, c);
	}

	{
		const TB::Matrix<2, 3, int> a((const int[6]){0, 1, 2, 3, 4, 5});
		const TB::Matrix<4, 2, int> b((const int[8]){0, 1, 2, 3, 4, 5, 6, 7});
		const TB::Matrix<4, 3, int> c(
			(const int[]){4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47});
		Check(a * b, c);
	}

	{
		const TB::Matrix<3, 3, int> a((const int[9]){1, 2, 3, 4, 5, 6});
		const TB::Vector<2, int> b((const int[2]){10, 11});
		const TB::Vector<2, int> c((const int[2]){35, 101});
		Check(2, a * b, c);
	}
	return 0;
}
