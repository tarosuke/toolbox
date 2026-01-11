#include <tb/geometry/rect.h>
#include <tb/test.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::geometry::Rect<2, int>& o) {
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
	tb::geometry::Rect<2, int> a(tb::geometry::Vector<2, int>({1, 2}),
		tb::geometry::Vector<2, int>({3, 4}));
	tb::geometry::Rect<2, int> b(tb::geometry::Vector<2, int>({0, 1}),
		tb::geometry::Vector<2, int>({2, 3}));
	tb::geometry::Rect<2, int> c(tb::geometry::Vector<2, int>({-1, -2}),
		tb::geometry::Vector<2, int>({4, 5}));
	tb::geometry::Rect<2, int> d(tb::geometry::Vector<2, int>({4, 5}),
		tb::geometry::Vector<2, int>({-1, -2}));

	assert(a && b);
	assert(a && c);
	assertEQ(a & c, a);
	assertEQ(d, c);

	tb::geometry::Rect<2, int> e(tb::geometry::Vector<2, int>({-1, -2}),
		tb::geometry::Spread<2, int>({5, 7}));
	assertEQ(e, c);

	tb::geometry::Rect<2, int> f(tb::geometry::Vector<2, int>({4, 5}),
		tb::geometry::Vector<2, int>({2, 3}));
	tb::geometry::Rect<2, int> g(tb::geometry::Vector<2, int>({4, 1}),
		tb::geometry::Vector<2, int>({2, 3}));
	tb::geometry::Rect<2, int> h(tb::geometry::Vector<2, int>({0, 5}),
		tb::geometry::Vector<2, int>({2, 3}));
	assert(!(b && f));
	assert(!(b && g));
	assert(!(b && h));

	tb::geometry::Rect<2, int> i(tb::geometry::Vector<2, int>({0, 0}),
		tb::geometry::Vector<2, int>({5, 6}));
	tb::geometry::Rect<2, int> j(tb::geometry::Spread<2, int>({5, 6}));
	assertEQ(i, j);

	return 0;
}
