#include <tb/image.h>
#include <tb/test.h>

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Color &c) : String('#') {
	Append(tb::u8(c.A()), '0', 2, 16);
	Append(tb::u8(c.R()), '0', 2, 16);
	Append(tb::u8(c.G()), '0', 2, 16);
	Append(tb::u8(c.B()), '0', 2, 16);
}

int main() {
	static tb::u32 buffer[2][2]{{0x0000ffff, 0xffff0000},
								{0xffff0000, 0x0000ffff}};
	tb::ImageARGB32 image((void *)&buffer[0][0], 2, 2);

	const tb::Color of(2.0f, 2.0f, 2.0f, 2.0f);
	const tb::Color uf(-1.0f, -1.0f, -1.0f, -1.0f);
	const tb::Color hf(0.5f, 0.5f, 0.5f, 0.5f);

	const tb::Color mb(255U, 255U, 255U, 255U);
	const tb::Color lb(0U, 0U, 0U, 0U);
	const tb::Color hb(127U, 127U, 127U, 127U);

	assertEQ(of, mb);
	assertEQ(uf, lb);
	assertEQ(hf, hb);

	const tb::Color p(image[0.5f][0.5f]);

	assertEQ(p, hb);
	assertEQ(hb, hb);

	return 0;
}
