#include <toolbox/test.h>
#include <toolbox/container/array.h>
#include <toolbox/type.h>

#include <stdio.h>



using I = TB::Type<int>;

namespace {
	void H(const TB::Array<int>& t) {
		printf("{ ");
		for (unsigned n(0); n < t.Length(); ++n) {
			printf("%d ", t[n]);
		}
		printf("}");
	}
	void HH(const TB::Array<I>& t) {
		printf("{ ");
		for (unsigned n(0); n < t.Length(); ++n) {
			printf("%d ", (int&)t[n]);
		}
		printf("}");
	}
}

int main() {
	const int test[]{-1, 0, 1, 2, 7, -1, 0, 1, 2, 7,
					 -1, 0, 1, 2, 7, -1, 0, 1, 2, 7};
	{
		TB::Array<int> a(test, 5);
		TB::Array<int> b(a);
		assertEQ(b, a, H);

		TB::Array<int> c(test, 10);
		b += b;
		assertEQ(b, c, H);

		TB::Array<int> d(test, 20);
		b += b;
		assertEQ(b, d, H);
	}

	{
		TB::Array<I> e;
		for (unsigned n(0); n < 5; ++n) {
			e += I(test[n]);
		}
		TB::Array<I> f(e);
		assertEQ(f, e, HH);

		TB::Array<I> g;
		for (unsigned n(0); n < 10; ++n) {
			g += I(test[n]);
		}
		f += f;
		assertEQ(f, g, HH);

		TB::Array<I> h;
		for (unsigned n(0); n < 20; ++n) {
			h += I(test[n]);
		}
		f += f;
		assertEQ(f, h, HH);
	}


	return 0;
}
