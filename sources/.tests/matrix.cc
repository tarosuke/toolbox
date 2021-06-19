#include <stdio.h>
#include <stdlib.h>

#include <toolbox/geometry/matrix.h>


using M22 = TB::Matrix<2, 2, int>;

template <unsigned R, unsigned C>
void Check(const TB::Matrix<C, R, int>& a, const TB::Matrix<C, R, int>& q) {
	if (a == q) {
		return;
	}

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

int main() {
	{
		const M22 a((const int[]){1, 2, 3, 4});
		const M22 b((const int[]){5, 6, 7, 8});
		const M22 c((const int[]){19, 22, 43, 50});
		Check(a * b, c);
	}

	{
		const TB::Matrix<2, 3, int> a((const int[]){0, 1, 2, 3, 4, 5});
		const TB::Matrix<4, 2, int> b((const int[]){0, 1, 2, 3, 4, 5, 6, 7});
		const TB::Matrix<4, 3, int> c(
			(const int[]){4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47});
		Check(a * b, c);
	}

	{
		const TB::Matrix<3, 3, int> a((const int[]){1, 2, 3, 4, 5, 6});
		const TB::Vector<2, int> b((const int[2]){7, 8});
		const TB::Vector<2, float> c((const int[2]){9, 10});
		if (a * b != c) {
			exit(-1);
		}
	}
	return 0;
}
