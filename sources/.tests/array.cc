#include <stdio.h>
#include <tb/array.h>
#include <tb/string.h>
#include <tb/test.h>



struct N : public tb::Array<N>::Node {
	N(tb::Array<N>& a) : Node(a){};
	bool operator==(uint t) { return t == ID(); };
	bool operator!=(uint t) { return t != ID(); };
};



template <> tb::String::String(const N& v) { Append(v.ID()); }
template <> tb::String::String(const int& o) { Append(o); }



int main() {
	tb::Array<N> arr;

	N* a(new N(arr));
	N* b(new N(arr));
	N* c(new N(arr));

	assertEQ(*a, 0);
	assertEQ(*b, 1);
	assertEQ(*c, 2);


	delete b;
	N* d(new N(arr));
	assertEQ(*a, 0);
	assertEQ(*d, 1);
	assertEQ(*c, 2);



	// const int test[]{-1, 0, 1, 2, 7, -1, 0, 1, 2, 7,
	// 				 -1, 0, 1, 2, 7, -1, 0, 1, 2, 7};
	// {
	// 	tb::Array<int> a(test, 5);
	// 	tb::Array<int> b(a);
	// 	assertEQ(b, a);

	// 	tb::Array<int> c(test, 10);
	// 	b += b;
	// 	assertEQ(b, c);

	// 	tb::Array<int> d(test, 20);
	// 	b += b;
	// 	assertEQ(b, d);
	// }

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
