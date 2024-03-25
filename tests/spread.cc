#include <tb/spread.h>
#include <tb/test.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Spread<2, int>& o) {
	Append(o[0], '0', 2, 16);
	push_back(',');
	Append(o[1], '0', 2, 16);
}


int main() {
	const tb::Spread<2, int> a(0, 0);
	const tb::Spread<2, int> b(-1, 1);
	tb::Spread<2, int> c(2, 2);

	assert(!a);
	assert(!b);
	assert(c);

	const tb::Spread<2, int> d(6, 6);
	const tb::Spread<2, int> e(1, 1);
	tb::Spread<2, int> f(c);
	assertEQ(c * 3, d);
	assertEQ(c / 2, e);
	assertEQ(f, c);

	c *= 3;
	assertEQ(c, d);

	f /= 2;
	assertEQ(f, e);

	return 0;
}
