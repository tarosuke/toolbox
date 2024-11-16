#include <tb/image.h>
#include <tb/test.h>

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Color &c) : String('#') {
	Append((tb::u32)c, '0', 8, 16);
}

int main() {
	static tb::u32 buffer[2][2]{{0x0000ffff, 0xffff0000},
								{0xffff0000, 0x0000ffff}};
	tb::ImageARGB32 image((void *)&buffer[0][0], 2, 2);

	const tb::Color of(1.0f, 1.0f, 1.0f, 1.0f);
	const tb::Color uf(0.0f, 0.0f, 0.0f, 0.0f);
	const tb::Color hf(0.5f, 0.5f, 0.5f, 0.5f);

	const tb::Color p(image.Get(0.5f, 0.5f));

	assertEQ(p, hf);
	// assertEQ(hb, hb);

	return 0;
}
