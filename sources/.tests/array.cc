#include <tb/array.h>
#include <tb/test.h>
// #include <toolbox/type.h>

#include <stdio.h>



// using I = tb::Type<int>;

template <> tb::String::String(const tb::Array<int>& v) {
	*this += "{ ";
	for (unsigned n(0); n < v.Length(); ++n) {
		Append(v[n]);
		*this += ", ";
	}
	*this += "}";
}
// template <> tb::String::String(const tb::Array<I>& v) {
// 	*this << "{ ";
// 	for (unsigned n(0); n < v.Length(); ++n) {
// 		*this << (int&&)v[n] << ", ";
// 	}
// 	*this << "}";
// }



int main() {
	const int test[]{-1, 0, 1, 2, 7, -1, 0, 1, 2, 7,
					 -1, 0, 1, 2, 7, -1, 0, 1, 2, 7};
	{
		tb::Array<int> a(test, 5);
		tb::Array<int> b(a);
		assertEQ(b, a);

		tb::Array<int> c(test, 10);
		b += b;
		assertEQ(b, c);

		tb::Array<int> d(test, 20);
		b += b;
		assertEQ(b, d);
	}

	// {
	// 	tb::Array<I> e;
	// 	for (unsigned n(0); n < 5; ++n) {
	// 		e += I(test[n]);
	// 	}
	// 	tb::Array<I> f(e);
	// 	assertEQ(f, e);

	// 	tb::Array<I> g;
	// 	for (unsigned n(0); n < 10; ++n) {
	// 		g += I(test[n]);
	// 	}
	// 	f += f;
	// 	assertEQ(f, g);

	// 	tb::Array<I> h;
	// 	for (unsigned n(0); n < 20; ++n) {
	// 		h += I(test[n]);
	// 	}
	// 	f += f;
	// 	assertEQ(f, h);
	// }


	return 0;
}
