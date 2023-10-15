#include <tb/prefs.h>
#include <tb/test.h>



static tb::Prefs<uint> a("--A", "試験A");
static tb::Prefs<uint> b("--B", 3, "試験B");
static tb::Prefs<uint> d("--C", 5, "試験C", tb::CommonPrefs::nosave);

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const uint& o) { Append(o); }

int main(int, char** argv) {
	{
		const char* v[]{argv[0], "--A=1", "--B", "2", "--C=6"};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)a, 1U);
		assertEQ((uint)b, 2U);
		assertEQ((uint)d, 6U);
		d = 5;
	}
	{
		const char* v[]{argv[0]};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)a, 1U);
		assertEQ((uint)b, 2U);
		b = 4;
		assertEQ((uint)b, 4U);
		assertEQ((uint)d, 5U);
	}
	{
		const char* v[]{argv[0]};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)b, 4U);
		assertEQ((uint)d, 5U);
	}


	return 0;
}
