#include <toolbox/test.h>
#include <toolbox/container/array.h>
#include <toolbox/type.h>

#include <stdio.h>



using I = TB::Type<int>;

template <> TB::String::String(const TB::Array<int>& v) {
	*this << "{ ";
	for (unsigned n(0); n < v.Length(); ++n) {
		*this << v[n] << ", ";
	}
	*this << "}";
}
template <> TB::String::String(const TB::Array<I>& v) {
	*this << "{ ";
	for (unsigned n(0); n < v.Length(); ++n) {
		*this << (int&&)v[n] << ", ";
	}
	*this << "}";
}



int main() {
	const int test[]{-1, 0, 1, 2, 7, -1, 0, 1, 2, 7,
					 -1, 0, 1, 2, 7, -1, 0, 1, 2, 7};
	{
		TB::Array<int> a(test, 5);
		TB::Array<int> b(a);
		assertEQ(b, a);

		TB::Array<int> c(test, 10);
		b += b;
		assertEQ(b, c);

		TB::Array<int> d(test, 20);
		b += b;
		assertEQ(b, d);
	}

	{
		TB::Array<I> e;
		for (unsigned n(0); n < 5; ++n) {
			e += I(test[n]);
		}
		TB::Array<I> f(e);
		assertEQ(f, e);

		TB::Array<I> g;
		for (unsigned n(0); n < 10; ++n) {
			g += I(test[n]);
		}
		f += f;
		assertEQ(f, g);

		TB::Array<I> h;
		for (unsigned n(0); n < 20; ++n) {
			h += I(test[n]);
		}
		f += f;
		assertEQ(f, h);
	}


	return 0;
}
