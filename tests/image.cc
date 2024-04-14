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



	const tb::Pixel<tb::f32> of((const float[4]){2.0f, 2.0f, 2.0f, 2.0f});
	const tb::Pixel<tb::f32> uf((const float[4]){-1.0f, -1.0f, -1.0f, -1.0f});
	const tb::Pixel<tb::f32> hf((const float[4]){0.5f, 0.5f, 0.5f, 0.5f});

	const tb::Pixel<tb::u8> mb(255U, 255U, 255U, 255U);
	const tb::Pixel<tb::u8> lb(0U, 0U, 0U, 0U);
	const tb::Pixel<tb::u8> hb(127U, 127U, 127U, 127U);


	assertEQ(tb::Pixel<tb::u8>(of), mb);
	assertEQ(tb::Pixel<tb::u8>(uf), lb);
	assertEQ(tb::Pixel<tb::u8>(hf), hb);


	const tb::Pixel<tb::u8> p(image[0.5f][0.5f]);
	const tb::Pixel<tb::u8> q(hb);

	assertEQ(p, hb);
	assertEQ(q, hb);

	return 0;
}
