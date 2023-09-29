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

#include <limits>
#include <tb/types.h>
#include <type_traits>



namespace tb {

	template <typename T> struct Pixel {
		static constexpr T max = std::numeric_limits<T>::max();

		Pixel(T v = 0) : arr{v, v, v, v} {};

		// Pixel(tb::u32 wc) // W:Tの4倍サイズの符号なし整数
		// 	: arr{(tb::u32)(wc >> (3 * sizeof(T) / 8)),
		// 		  (tb::u32)(wc >> (2 * sizeof(T) / 8)),
		// 		  (tb::u32)(wc >> (1 * sizeof(T) / 8)),
		// 		  (tb::u32)wc} {}
		Pixel(const T (&iv)[4]) : arr{iv[0], iv[1], iv[2], iv[3]} {};
		Pixel(T a, T b, T c, T d) : arr{a, b, c, d} {};

		template <typename U> Pixel(T a, T b, T c, T d)
			: arr{Limit(a), Limit(b), Limit(c), Limit(d)} {};
		template <typename U> Pixel(const U (&o)[4])
			: arr{Limit(o[0]), Limit(o[1]), Limit(o[2]), Limit(o[3])} {};

		// 異なる型のPixel変換
		template <typename U> Pixel(const Pixel<U>& o) : Pixel(o.GetArray()){};

		// 内部へのアクセス
		const T (&GetArray() const)[4] { return arr; };

		// 演算子
		bool operator==(const Pixel& o) const {
			return arr[0] == o.arr[0] && arr[1] == o.arr[1] &&
				   arr[2] == o.arr[2] && arr[3] == o.arr[3];
		};
		bool operator!=(const Pixel& o) const { return !(*this == o); };

		template <typename U> Pixel<U> operator*(U v) const {
			static_assert(
				std::is_floating_point<U>::value,
				"引数は実数でなければならない。");
			Pixel<U> r(arr);
			r *= v;
			return r;
		};
		template <typename U> Pixel& operator*=(U u) {
			static_assert(
				std::is_floating_point<U>::value,
				"引数は実数でなければならない。");
			arr[0] *= u;
			arr[1] *= u;
			arr[2] *= u;
			arr[3] *= u;
			return *this;
		};
		Pixel operator+(Pixel t) const {
			return Pixel{
				arr[0] + t.arr[0],
				arr[1] + t.arr[1],
				arr[2] + t.arr[2],
				arr[3] + t.arr[3]};
		};

		T operator[](uint n) const { return arr[n]; };

	private:
		T arr[4]; // 順番は知らない
		template <typename U> static T Limit(U u) {
			if constexpr (std::is_same_v<T, U>) {
				// 同じ：そのまま
				return u;
			} else if constexpr (
				std::is_unsigned_v<T> && std::is_unsigned_v<U>) {
				// サイズの異なる整数間：最大サイズの整数でブレゼンハム
				const u128 uu((u128)u * Pixel<T>::max / Pixel<U>::max);
				return max < uu ? max : (T)uu;
			} else if constexpr (
				std::is_floating_point_v<T> && std::is_floating_point_v<U>) {
				// 実数同士：範囲だけ補正
				return u < (U)0 ? (U)0 : (U)1 < u ? 1 : u;
			} else if constexpr (
				std::is_unsigned_v<T> && std::is_floating_point_v<U>) {
				// 実数から整数：maxをかけて範囲補正
				const U vv(u * max);
				return vv < 0 ? 0 : max < vv ? max : vv;
			} else if constexpr (
				// 整数から実数：0-1へ変換
				std::is_floating_point_v<T> && std::is_unsigned_v<U>) {
				return (T)u / Pixel<U>::max;
			}
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
			: buffer((P*)buffer), width(width), height(height){};

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

			template <typename U> const Pixel<float> operator[](U x) {
				const auto hr(HeadsAndRatios(x, width));
				return Pixel<U>(
					heads[0][hr.heads[0]] * (ratios[0] * hr.ratios[0]) +
					heads[1][hr.heads[0]] * (ratios[1] * hr.ratios[0]) +
					heads[0][hr.heads[1]] * (ratios[0] * hr.ratios[1]) +
					heads[1][hr.heads[1]] * (ratios[1] * hr.ratios[1]));
			};

		private:
			P* const heads[2];
			const float ratios[2];
			const unsigned width;
		};

		Line operator[](unsigned y) { return Line(buffer + width * y); };
		Lines operator[](f32 y) {
			const auto hr(HeadsAndRatios(y, height));
			P* const h[2]{
				buffer + hr.heads[0] * width,
				buffer + hr.heads[1] * width};

			return Lines(h, hr.ratios, width);
		};

	private:
		P* const buffer;
		const unsigned width;
		const unsigned height;

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
