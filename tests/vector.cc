#include <tb/geometry/vector.h>
#include <tb/test.h>



// 試験用比較演算子

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::geometry::Vector<3, int>& c) {
	Append(c[0]);
	push_back(',');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
}
template <> tb::String::String(const int& o) { Append(o); }



int main() {
	const tb::geometry::Vector<3, int> a({0, 1, 2});
	const tb::geometry::Vector<3, int> b({2, 1, 0});
	const tb::geometry::Vector<3, int> c({2, 2, 2});

	assertNE(a, b);
	assertEQ(a + b, c);
	assertEQ(c - a, b);

	const tb::geometry::Vector<3, int> d({4, 4, 4});

	assertEQ(c * 2, d);
	assertEQ(d / 2, c);

	const tb::geometry::Vector<3, int> e({1, 0, 0});
	const tb::geometry::Vector<3, int> f({0, 1, 0});
	const tb::geometry::Vector<3, int> g({0, 0, 1});

	assertEQ(e.Cross(f), g);

	const tb::geometry::Vector<2, int> h({0, 2});
	const tb::geometry::Vector<2, int> i({2, 0});

	assertEQ(Cross2(h, i), -4);


	const tb::geometry::Vector<3, int> j({1, 2, 3});
	const tb::geometry::Vector<3, int> k({4, 5, 6});

	assertEQ(j * k, 32);


	return 0;
}
