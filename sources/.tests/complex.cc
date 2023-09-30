#include <tb/complex.h>
#include <tb/test.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Complex<2, int>& c) {
	Append(c[0]);
	push_back(':');
	Append(c[1]);
}
template <> tb::String::String(const tb::Complex<4, int>& c) {
	Append(c[0]);
	push_back(':');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
	push_back(',');
	Append(c[3]);
}
template <> tb::String::String(const int& o) { Append(o); }



int main() {

	tb::Complex<4, int> a((const int[4]){1, 2, 3, 4});

	assertEQ(a[0], 1);
	assertEQ(a[1], 2);
	assertEQ(a[2], 3);
	assertEQ(a[3], 4);

	assertEQ(a.Norm2(), 27);
	assertEQ(a.Norm(), 5);

	const tb::Complex<4, int> b((const int[4]){4, 8, 12, 16});
	const tb::Complex<4, int> c((const int[4]){2, 4, 6, 8});

	a *= 4;
	assertEQ(a, b);
	a /= 2;
	assertEQ(a, c);

	return 0;
}
