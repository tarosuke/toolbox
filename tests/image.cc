#include <tb/image.h>
#include <tb/test.h>

// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Color& c) : String('#') {
	Append(c.WebColor(), '0', 8, 16);
}

// 色比較
bool operator==(const tb::Color& a, const tb::Color& b) {
	return a.WebColor() == b.WebColor();
}


int main() {
	static tb::u32 buffer[2][2]{
		{0x0000ffff, 0xffff0000}, {0xffff0000, 0x0000ffff}};
	tb::Image image((void*)&buffer[0][0],
		tb::Color::Format::Select(tb::Color::Format::ARGB8888), 2, 2, 8);

	const tb::Color of(1.0f, 1.0f, 1.0f, 1.0f);
	const tb::Color uf(0.0f, 0.0f, 0.0f, 0.0f);
	const tb::Color hf(0.5f, 0.5f, 0.5f, 0.5f);

	const tb::Color p(image.Get(0.5f, 0.5f));

	assertEQ(p, hf);
	// assertEQ(hb, hb);

	return 0;
}
