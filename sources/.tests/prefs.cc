#include <tb/prefs.h>
#include <tb/test.h>



static tb::Prefs<uint> a("--A", "試験A");
static tb::Prefs<uint> b("--B", 3, "試験B");

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const uint& o) { Append(o); }

int main(int, char** argv) {
	{
		const char* v[]{argv[0], "--A=1", "--B", "2"};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)a, 1U);
		assertEQ((uint)b, 2U);
	}


	return 0;
}
