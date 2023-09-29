#include <tb/test.h>
#include <tb/vector.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Vector<3, int>& c) {
	Append(c[0]);
	push_back(',');
	Append(c[1]);
	push_back(',');
	Append(c[2]);
}



int main() {
	const tb::Vector<3, int> a((const int[]){0, 1, 2});
	const tb::Vector<3, int> b((const int[]){2, 1, 0});
	const tb::Vector<3, int> c((const int[]){2, 2, 2});

	assertNE(a, b);
	assertEQ(a + b, c);
	assertEQ(c - a, b);

	const tb::Vector<3, int> d((const int[]){4, 4, 4});

	assertEQ(c * 2, d);
	assertEQ(d / 2, c);

	const tb::Vector<3, int> e((const int[]){1, 0, 0});
	const tb::Vector<3, int> f((const int[]){0, 1, 0});
	const tb::Vector<3, int> g((const int[]){0, 0, 1});

	assertEQ(e.Cross(f), g);

	return 0;
}
