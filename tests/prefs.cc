#include <tb/prefs.h>
#include <tb/string.h>
#include <tb/test.h>



static tb::Prefs<uint> a("--A", "試験A");
static tb::Prefs<uint> b("--B", 3, "試験B");
static tb::Prefs<uint> d("--D", 5, "試験D", tb::CommonPrefs::nosave);
static tb::Prefs<tb::String> e("--E", "abc", "試験E");



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const uint& o) { Append(o); }

int main(int, char** argv) {
	{
		const char* v[]{argv[0], "--A=1", "--B", "2", "--D=6"};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)a, 1U);
		assertEQ((uint)b, 2U);
		assertEQ((uint)d, 6U);
		d = 5;
		assertEQ((const tb::String&)e, tb::String("abc"));
		e = tb::String("abc_def");
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
		assertEQ((const tb::String&)e, tb::String("abc_def"));
		e = tb::String("abc");
	}
	{
		const char* v[]{argv[0]};
		int c(elementsOf(v));
		tb::CommonPrefs::Keeper k(c, v, 0);
		assertEQ((uint)b, 4U);
		assertEQ((uint)d, 5U);
		assertEQ((const tb::String&)e, tb::String("abc"));
	}


	return 0;
}
