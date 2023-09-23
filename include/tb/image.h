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

#include <tb/types.h>



namespace tb {

	template <typename T> struct Pixel {
		Pixel(T v = 0) : arr{v, v, v, v} {};
		template <typename W> Pixel(W wc) // W:Tの4倍サイズの符号なし整数
			: arr{wc >> (3 * sizeof(T) / 8),
				  wc >> (2 * sizeof(T) / 8),
				  wc >> (1 * sizeof(T) / 8),
				  wc} {}
		Pixel(const T iv[4]) : arr{iv[0], iv[1], iv[2], iv[3]} {};

		bool operator==(const Pixel& o) const {
			return arr[0] == o.arr[0] && arr[1] == o.arr[1] &&
				   arr[2] == o.arr[2] && arr[3] == o.arr[3];
		};
		bool operator!=(const Pixel& o) const { return !(*this == o); };

		// 実数表現へ変換
		operator Pixel<float>() const {
			return Pixel<float>{
				(float)arr[0] / max,
				(float)arr[1] / max,
				(float)arr[2] / max,
				(float)arr[3] / max};
		};

		// 整数表現へ変換
		template <typename U> operator Pixel<U>() const {
			return Pixel<U>{
				Pixel<U>::max * arr[0],
				Pixel<U>::max * arr[1],
				Pixel<U>::max * arr[2],
				Pixel<U>::max * arr[3]};
		};

		Pixel operator*(T v) const {
			return Pixel{arr[0] * v, arr[1] * v, arr[2] * v, arr[3] * v};
		};
		Pixel operator+(Pixel t) const {
			return Pixel{
				arr[0] + t.arr[0],
				arr[1] + t.arr[1],
				arr[2] + t.arr[2],
				arr[3] + t.arr[3]};
		};

	private:
		static constexpr T max = ((T)~0U);
		T arr[4]; // 順番は知らない
	};

	namespace ImageBase {
		struct HR {
			unsigned heads[2]; // 配列の添字
			f32 ratios[2]; // 補間用のレート(2つを足すと1になる)
		};
		// positionから整数部の添字と補間用の値を作る
		HR HeadsAndRatios(f32 position, uint limit) {
			int i(position); // 整数部
			const float r(position - i); // 小数点以下(必ず正)

			// 整数部を正側へシフト
			const unsigned u((unsigned)(i % limit + limit));

			// 結果を作って返す
			return HR{{u % limit, (u + 1) % limit}, {1.0f - r, r}};
		};
	};

	template <typename P = Pixel<u8>> struct Image {
		Image() = delete;
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		/***** Imageインターフェイスの構築子
		 * Imageは引数の範囲検証などはしないので引数も機能を提供できる最低限
		 * になっている。二次元配列のようにアクセスする。
		 * NOTE:補間機能を使う場合はheightを指定すること。
		 */
		Image(void* buffer, unsigned width, unsigned height = 0)
			: width(width), height(height), buffer(buffer){};

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
			Lines(ImageBase::HR hr, unsigned width)
				: heads{hr.heads[0], hr.heads[1]},
				  ratios{hr.ratios[0], hr.ratios[1]}, width(width){};

			const P operator[](float x) {
				const auto hr(ImageBase::HeadsAndRatios(x, width));
				return heads[0][hr.heads[0]] * ratios[0] * hr.ratios[0] +
					   heads[1][hr.heads[0]] * ratios[1] * hr.ratios[0] +
					   heads[0][hr.heads[1]] * ratios[0] * hr.ratios[1] +
					   heads[1][hr.heads[1]] * ratios[1] * hr.ratios[1];
			};

		private:
			P* const heads[2];
			const float ratios[2];
			const unsigned width;
		};

		Line operator[](unsigned y) { return Line(buffer + width * y); };
		Lines operator[](f32 y) {
			return Lines(ImageBase::HeadsAndRatios(y, height), width);
		};

	private:
		P* const buffer;
		const unsigned width;
		const unsigned height;
	};
}
