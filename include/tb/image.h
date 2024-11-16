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
#include <concepts>
#include <limits>
#include <tb/rect.h>
#include <tb/spread.h>
#include <tb/types.h>

namespace tb {

	struct Color {
		friend bool operator==(const Color &, const Color &);

		Color() = default;
		Color(const Color &) = default;
		explicit Color(u32 webColor)
			: Color(u8(webColor >> 24), u8(webColor >> 16), u8(webColor >> 8),
					u8(webColor)) {};
		explicit Color(float a, float r, float g, float b) : e{a, r, g, b} {};

		float operator-(const Color &t) const {
			const float r[4]{e[0] - t.e[0], e[1] - t.e[1], e[2] - t.e[2],
							 e[3] - t.e[3]};
			return r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3];
		};

		Color Learp(const Color t, float ratio) {
			const float rr(1 - ratio);
			return Color(e[0] * rr + t.e[0] * ratio, e[1] * rr + t.e[1] * ratio,
						 e[2] * rr + t.e[2] * ratio,
						 e[3] * rr + t.e[3] * ratio);
		};

		const float &A() const { return e[0]; };
		const float &R() const { return e[1]; };
		const float &G() const { return e[2]; };
		const float &B() const { return e[3]; };

		operator u32() const {
			return ((u32)(255 * A()) << 24) | ((u32)(255 * R()) << 16) |
				   ((u32)(255 * G()) << 8) | (u32)(255 * B());
		};

	private:
		float e[4];
	};
	bool operator==(const Color &o, const Color &t);

	/***** 画像インターフェイス
	 * 既存の画像データを取り出して扱うための抽象クラス
	 */
	struct Image {
		struct Profile {
			struct E {
				u32 bit;
				u32 mask;
				template <typename T> float F(T v) const {
					return float((v >> bit) & mask) / mask;
				};
				u32 U(float v) const { return u32(v * mask) << bit; };
			} elements[4];
			unsigned bitsPerPixel;
			bool IsTransparent() { return !!elements[0].mask; };
			Color C(const u8 *left, unsigned v) const {
				const u32 &t(*(const u32 *)&left[v * bitsPerPixel / 8]);
				return Color(elements[0].F(t), elements[1].F(t),
							 elements[2].F(t), elements[3].F(t));
			};
		};

		Image() = delete;
		Image(const Image &) = delete;
		void operator=(const Image &) = delete;

		virtual ~Image() {};

		// 生データ
		void *Data() const { return (void *)buffer; };
		unsigned Width() const { return width; };
		unsigned Height() const { return height; };
		bool Transparent() const { return !!profile.elements[0].mask; }
		tb::Spread<2, int> Spread() const {
			return tb::Spread<2, int>((int[]){(int)width, (int)height});
		};

		Color Get(unsigned x, unsigned y) const;
		Color Get(float x, float y) const;

	protected:
		u8 *const buffer;

		/***** Imageインターフェイスの構築子
		 * 特クラスから貰った諸元を記録するだけ
		 */
		Image(void *buffer, const Image::Profile &profile, unsigned width,
			  unsigned height, unsigned stride)
			: buffer((u8 *)buffer), profile(profile), width(width),
			  height(height), stride(stride) {};

		/***** 与えられたImageの一部を複製
		 */
		Image(void *buffer, const Image &org, unsigned left, unsigned top,
			  unsigned width, unsigned height, unsigned stride);

	private:
		const Image::Profile &profile;
		const unsigned width;  // [px]
		const unsigned height; // [px]
		const unsigned stride; // [bytes]

		u8 *Left(unsigned y) const { return buffer + (y % height) + stride; };
	};

	// ARGB32
	struct ImageARGB32 : public Image {
		ImageARGB32(void *buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};
		static const Image::Profile profile;
	};
	// xRGB32
	struct ImageXRGB32 : public Image {
		ImageXRGB32(void *buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};
		static const Image::Profile profile;
	};

	/***** バッファ自動管理Image
	 * 普通のImageはすでにあるImageを扱うことを意図しているが、BufferedImageは
	 * バッファ自体も管理することを想定している
	 * NOTE:テンプレート引数TにはImageARGB32のような具象Image型を与える
	 */
	template <class T> struct BufferedImage : public T {
		BufferedImage(unsigned width, unsigned height)
			: T(new tb::u32[width * height], width, height) {};
		BufferedImage(const Image &origin, tb::Vector<2, int> &offset,
					  unsigned width, unsigned height)
			: Image(new tb::u32[width * height], origin, offset[0], offset[1],
					width, height) {};

		~BufferedImage() { delete[] (tb::u32 *)Image::buffer; };
	};
}
