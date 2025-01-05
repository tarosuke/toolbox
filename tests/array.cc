#include <stdio.h>
#include <tb/array.h>
#include <tb/string.h>
#include <tb/test.h>



struct N : public tb::Array<N>::Node {
	// N(tb::Array<N>& a) : Node(a){};
	N(tb::Array<N>& a, uint id) : Node(a, id) {};
	bool operator==(uint t) { return t == GetID(); };
	bool operator!=(uint t) { return t != GetID(); };
};



template <> tb::String::String(const N& v) { Append(v.GetID()); }
template <> tb::String::String(const int& o) { Append(o); }



int main() {
	// { この項目はTableへ分離された
	// 	tb::Array<N> arr;

	// 	N* a(new N(arr));
	// 	N* b(new N(arr));
	// 	N* c(new N(arr));

	// 	assertEQ(*a, 0);
	// 	assertEQ(*b, 1);
	// 	assertEQ(*c, 2);


	// 	delete b;
	// 	N* d(new N(arr));
	// 	assertEQ(*a, 0);
	// 	assertEQ(*d, 1);
	// 	assertEQ(*c, 2);
	// }
	tb::Array<N> arr;
	N e(arr, 1);
	N f(arr, 10);
	N g(arr, 100);
	assertEQ(e, 1);
	assertEQ(f, 10);
	assertEQ(g, 100);

	return 0;
}
