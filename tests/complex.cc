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
template <> tb::String::String(const tb::Complex<4, float, true>& c) {
	Append(c[0]);
	push_back(':');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
	push_back(',');
	Append(c[3]);
}
template <> tb::String::String(const tb::Vector<3, int>& c) {
	Append(c[0]);
	push_back(',');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
}
template <> tb::String::String(const tb::Vector<3, float, true>& c) {
	Append(c[0]);
	push_back(',');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
}
template <> tb::String::String(const int& o) { Append(o); }



int main() {

	tb::Complex<4, int> a((const int[4]){1, 2, 3, 4});

	assertEQ(a[0], 1);
	assertEQ(a[1], 2);
	assertEQ(a[2], 3);
	assertEQ(a[3], 4);

	assertEQ(a.Norm2(), 30);
	assertEQ(a.Norm(), 5);

	const tb::Complex<4, int> b((const int[4]){4, 8, 12, 16});
	const tb::Complex<4, int> c((const int[4]){2, 4, 6, 8});

	a *= 4;
	assertEQ(a, b);
	a /= 2;
	assertEQ(a, c);

	const tb::Complex<4, float, true> d((const float[3]){1, 0, 0}, 1.0f);
	const tb::Vector<3, float, true> e(0.0f, 1.0f, 0.0f);
	const tb::Vector<3, float, true> f(0.0f, cosf(1.0f), sinf(1.0f));
	const tb::Vector<3, float, true> g(0.0f, 0.0f, 1.0f);
	const tb::Vector<3, float, true> h(1.0f, 0.0f, 0.0f);
	const tb::Vector<3, float, true> i(0.0f, -sinf(1.0f), cosf(1.0f));
	assertEQ(d.Rotate(e), f);
	assertEQ(d.Rotate(g), i);
	assertEQ(d.Rotate(h), h);


	return 0;
}
