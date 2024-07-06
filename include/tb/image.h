/***** 画像の直接操作インターフェイス
 * Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#pragma once

#include <algorithm>
#include <limits>
#include <tb/spread.h>
#include <tb/types.h>
#include <type_traits>



namespace tb {

	/***** 変換やらのための画素データ
	 */
	template <typename T> struct Pixel {};

	template <> struct Pixel<tb::u8> {
		Pixel(tb::u32 c = 0) : color(c){};
		Pixel(tb::u8 a, tb::u8 r, tb::u8 g, tb::u8 b)
			: color(
				  ((tb::u32)a << 24) | ((tb::u32)r << 16) | ((tb::u32)g << 8) |
				  (tb::u32)b){};
		Pixel(const float (&c)[4]) : color(F2u(c)){};
		Pixel(const tb::u8 (&c)[4]) : color(A2u(c)){};
		Pixel& operator=(const Pixel&) = default;
		operator const tb::u32&() { return color; };

		template <typename U> tb::u8 operator[](U n) const {
			return color >> (8 * n);
		};
		bool operator==(const Pixel& t) const { return color == t.color; };

	private:
		tb::u32 color;

		static tb::u32 F2p(float p) {
			return std::clamp((tb::u32)p * 255, 0U, 255U);
		};
		static tb::u32 F2u(const float (&c)[4]) {
			return (F2p(c[0]) << 24) | (F2p(c[1]) << 16) | (F2p(c[2]) << 8) |
				   F2p(c[3]);
		};
		static tb::u32 A2u(const tb::u8 (&c)[4]) {
			return ((tb::u32)c[0] << 24) | ((tb::u32)c[1] << 16) |
				   ((tb::u32)c[2] << 8) | (tb::u32)c[3];
		};
	};

	template <> struct Pixel<float> {
		Pixel(tb::u32 c)
			: color{
				  ((c >> 24) & 255) / 255.0f,
				  ((c >> 16) & 255) / 255.0f,
				  ((c >> 8) & 255) / 255.0f,
				  (c & 255) / 255.0f} {};
		Pixel(tb::u8 a, tb::u8 r, tb::u8 g, tb::u8 b)
			: color{U2f(a), U2f(r), U2f(g), U2f(b)} {};
		Pixel(float a, float r, float g, float b) : color{a, r, g, b} {};
		Pixel(const float (&c)[4]) : color{c[0], c[1], c[2], c[3]} {};
		Pixel(const tb::u8 (&c)[4])
			: color{U2f(c[0]), U2f(c[1]), U2f(c[2]), U2f(c[3])} {};
		operator tb::u32() const {
			return ((*this)[0] << 24) | ((*this)[1] << 16) | ((*this)[2] << 8) |
				   (*this)[3];
		};
		template <typename U> tb::u8 operator[](U n) const {
			return (tb::u32)std::clamp(color[n] * 255.0f, 0.0f, 255.0f);
		};
		Pixel operator*(float t) {
			return Pixel{
				color[0] * t,
				color[1] * t,
				color[2] * t,
				color[3] * t};
		};
		Pixel operator+(Pixel t) {
			return Pixel{
				color[0] + t.color[0],
				color[1] + t.color[1],
				color[2] + t.color[2],
				color[3] + t.color[3]};
		};

	private:
		float color[4];
		static float U2f(tb::u8 c) { return c / 255.0f; };
	};


	/***** 画像インターフェイス
	 * 既存の画像データを取り出して扱うためのクラス
	 */
	template <typename P> struct Image {
		Image() = delete;
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		/***** Imageインターフェイスの構築子
		 * Imageは引数の範囲検証などはしないので引数も機能を提供できる最低限
		 * になっている。二次元配列のようにアクセスする。
		 * NOTE:補間機能を使う場合はheightを指定すること。
		 */
		Image(
			void* buffer,
			unsigned width,
			unsigned height = 0,
			bool transparent = false)
			: buffer((P*)buffer), spread((int)width, (int)height),
			  transparent(transparent){};
		Image(
			void* buffer,
			const Spread<2, int>& spread,
			bool transparent = false)
			: buffer((P*)buffer), spread(spread), transparent(transparent){};

		/***** 一行分の画像
		 */
		struct Line {
			Line() = delete;
			Line(const Line&) = delete;
			Line(P* head) : head(head){};

			P& operator[](unsigned x) { return head[x]; };

		private:
			P* const head;
		};

		/***** 補間用の二行分
		 */
		struct Lines {
			Lines() = delete;
			Lines(const Line&) = delete;
			Lines(P* const h[2], const float (&r)[2], unsigned width)
				: heads{h[0], h[1]}, ratios{r[0], r[1]}, width(width){};

			template <typename U> const Pixel<float> operator[](U x) const {
				const auto hr(HeadsAndRatios(x, width));
				return Pixel<float>(heads[0][hr.heads[0]]) *
						   (ratios[0] * hr.ratios[0]) +
					   Pixel<float>(heads[1][hr.heads[0]]) *
						   (ratios[1] * hr.ratios[0]) +
					   Pixel<float>(heads[0][hr.heads[1]]) *
						   (ratios[0] * hr.ratios[1]) +
					   Pixel<float>(heads[1][hr.heads[1]]) *
						   (ratios[1] * hr.ratios[1]);
			};

		private:
			P* const heads[2];
			const float ratios[2];
			const unsigned width;
		};

		Line operator[](unsigned y) { return Line(buffer + spread[0] * y); };
		Lines operator[](f32 y) {
			const auto hr(HeadsAndRatios(y, spread[1]));
			P* const h[2]{
				buffer + hr.heads[0] * spread[0],
				buffer + hr.heads[1] * spread[0]};

			return Lines(h, hr.ratios, spread[0]);
		};


		// 直接アクセスのための情報取得
		void* Data() { return (void*)buffer; };
		const void* Data() const { return (void*)buffer; };
		unsigned Width() const { return spread[0]; };
		unsigned Height() const { return spread[1]; };
		unsigned Step() const { return spread[0]; };
		bool Transparent() const { return transparent; };

	private:
		P* const buffer;
		const Spread<2, int> spread;
		const bool transparent;

		struct HR {
			unsigned heads[2]; // 配列の添字
			f32 ratios[2]; // 補間用のレート(2つを足すと1になる)
		};
		// positionから整数部の添字と補間用の値を作る
		static HR HeadsAndRatios(f32 position, uint limit) {
			int i(position); // 整数部
			const float r(position - i); // 小数点以下(必ず正)

			// 整数部を正側へシフト
			const unsigned u((unsigned)(i % limit + limit));

			// 結果を作って返す
			return HR{{u % limit, (u + 1) % limit}, {1.0f - r, r}};
		};
	};
}
