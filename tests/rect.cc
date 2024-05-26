#include <tb/rect.h>
#include <tb/test.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Rect<2, int>& o) {
	const auto& l(o.Left());
	const auto& r(o.Right());

	Append(l[0], '0', 2, 16);
	push_back(',');
	Append(l[1], '0', 2, 16);
	push_back(',');
	Append(r[0], '0', 2, 16);
	push_back(',');
	Append(r[1], '0', 2, 16);
}


int main() {
	tb::Rect<2, int> a(tb::Vector<2, int>(1, 2), tb::Vector<2, int>(3, 4));
	tb::Rect<2, int> b(tb::Vector<2, int>(0, 1), tb::Vector<2, int>(2, 3));
	tb::Rect<2, int> c(tb::Vector<2, int>(-1, -2), tb::Vector<2, int>(4, 5));
	tb::Rect<2, int> d(tb::Vector<2, int>(4, 5), tb::Vector<2, int>(-1, -2));

	assert(a && b);
	assert(a && c);
	assertEQ(a & c, a);
	assertEQ(d, c);

	tb::Rect<2, int> e(tb::Vector<2, int>(-1, -2), tb::Spread<2, int>(5, 7));
	assertEQ(e, c);

	tb::Rect<2, int> f(tb::Vector<2, int>(4, 5), tb::Vector<2, int>(2, 3));
	tb::Rect<2, int> g(tb::Vector<2, int>(4, 1), tb::Vector<2, int>(2, 3));
	tb::Rect<2, int> h(tb::Vector<2, int>(0, 5), tb::Vector<2, int>(2, 3));
	assert(!(b && f));
	assert(!(b && g));
	assert(!(b && h));

	tb::Rect<2, int> i(tb::Vector<2, int>(0, 0), tb::Vector<2, int>(5, 6));
	tb::Rect<2, int> j(tb::Spread<2, int>(5, 6));
	assertEQ(i, j);

	return 0;
}
