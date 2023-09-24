#include <tb/image.h>
#include <tb/test.h>



// ダンプ用コピーコンストラクタ
template <> tb::String::String(const tb::Pixel<tb::u8>& c) : String('#') {
	Append(c[0], '0', 2, 16);
	Append(c[1], '0', 2, 16);
	Append(c[2], '0', 2, 16);
	Append(c[3], '0', 2, 16);
}


int main() {
	static tb::Pixel<tb::u8> buffer[2][2]{
		{(const tb::u8[4]){0, 0, 255, 255}, (const tb::u8[4]){255, 255, 0, 0}},
		{(const tb::u8[4]){255, 255, 0, 0}, (const tb::u8[4]){0, 0, 255, 255}}};
	tb::Image<tb::Pixel<tb::u8>> image(&buffer[0][0], 2, 2);


	const tb::Pixel<tb::u8> p(image[0.5f][0.5]);

	const tb::Pixel<tb::u8> ans(128);

	assertEQ(p, ans);


	return 0;
}
