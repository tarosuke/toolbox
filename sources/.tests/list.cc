#include <tb/list.h>
#include <tb/test.h>



namespace {
	struct Test : tb::List<Test>::Node {
		Test(int n) : v(n){};
		void Detach() { tb::List<Test>::Node::Detach(); };
		const int v;
	};
}


// ダンプ用コピーコンストラクタ
template <> tb::String::String(const int& o) { Append(o); }



int main() {
	Test a(0);
	Test b(1);

	// Attachして順番に
	{
		tb::List<Test> list;
		list.Add(a);
		list.Add(b);
		tb::List<Test>::I i(list);
		++i;
		assertEQ((*i).v, 0);
		++i;
		assertEQ((*i).v, 1);
	}

	// 前のノードを外した先頭
	{
		tb::List<Test> list;
		list.Add(a);
		list.Add(b);
		a.Detach();

		tb::List<Test>::I i(list);
		++i;
		assertEQ((*i).v, 1);
	}

	return 0;
};
