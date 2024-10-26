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
		struct E {
			friend class Color;

			E() : e(0) {};
			template <std::floating_point R> E(R v) : e(v){};
			template <std::unsigned_integral U> E(U v) : e(U(v)){};
			E &operator=(float v) {
				e = v;
				return *this;
			};
			E &operator=(u8 v) {
				e = U(v);
				return *this;
			};
			operator float() const { return e; };
			operator u8() const { return e * 255; };

		private:
			float e;
			static float U(u8 v) { return ((float)v) / 255; };
			E Learp(const E t, float ratio) {
				return E(e * ratio + t.e * (1 - ratio));
			};
		};

		Color() = default;
		Color(const Color &) = default;
		explicit Color(u32 webColor)
			: Color(u8(webColor >> 24), u8(webColor >> 16), u8(webColor >> 8),
					u8(webColor)) {};
		explicit Color(E a, E r, E g, E b) : e{a, r, g, b} {};
		template <typename T>
		Color(T a, T r, T g, T b) : e{E(a), E(r), E(g), E(b)} {};

		explicit operator u32() const {
			return ((u32)(u8)e[0] << 24) | ((u32)(u8)e[1] << 16) |
				   ((u32)(u8)e[2] << 8) | (u32)(u8)e[3];
		};
		operator const E *() { return e; };
		bool operator==(const Color &t) const {
			return (u32) * this == (u32)t;
		};

		Color Learp(const Color t, float ratio) {
			return Color(e[0].Learp(t.e[0], ratio), e[1].Learp(t.e[1], ratio),
						 e[2].Learp(t.e[2], ratio), e[3].Learp(t.e[3], ratio));
		};

		const E &A() const { return e[0]; };
		const E &R() const { return e[1]; };
		const E &G() const { return e[2]; };
		const E &B() const { return e[3]; };

	private:
		E e[4];
	};

	/***** 画像インターフェイス
	 * 既存の画像データを取り出して扱うための抽象クラス
	 */
	struct Image {
		struct Profile {
			struct {
				u32 bit;
				u32 mask;
			} a, r, g, b;
			unsigned bitsPerPixel;
			bool IsTransparent() { return !!a.mask; };
		};

		Image() = delete;
		Image(const Image &) = delete;
		void operator=(const Image &) = delete;

		virtual ~Image() {};

		// 生データ
		void *Data() const { return (void *)buffer; };
		unsigned Width() const { return width; };
		unsigned Height() const { return height; };
		bool Transparent() const { return !!profile.a.mask; }
		tb::Spread<2, int> Spread() const {
			return tb::Spread<2, int>((int[]){(int)width, (int)height});
		};

		// 一行分
		struct Line {
			Line() = delete;
			Line(void *left, const Profile &profile, unsigned width)
				: left(left), profile(profile), width(width) {};

			Color operator[](unsigned);
			operator void *() { return left; };

		private:
			void *left;
			const Profile &profile;
			const unsigned width; // [px]
		};
		Line operator[](unsigned);

		// 補間用に二行分
		struct Lines {
			Lines() = delete;
			Lines(void *left0, void *left1, const Profile &profile,
				  unsigned width)
				: lines{{left0, profile, width}, {left1, profile, width}} {};
			Color operator[](float);

		private:
			Line lines[2];
		};
		Lines operator[](float);

	protected:
		char *const buffer;

		/***** Imageインターフェイスの構築子
		 * 特クラスから貰った諸元を記録するだけ
		 */
		Image(void *buffer, const Profile &profile, unsigned width,
			  unsigned height, unsigned stride)
			: buffer((char *)buffer), profile(profile), width(width),
			  height(height), stride(stride) {};

		/***** 与えられたImageの一部を複製
		 */
		Image(void *buffer, const Image &org, unsigned left, unsigned top,
			  unsigned width, unsigned height, unsigned stride);

	private:
		const Profile &profile;
		const unsigned width;  // [px]
		const unsigned height; // [px]
		const unsigned stride; // [bytes]
	};

	// ARGB32
	struct ImageARGB32 : public Image {
		ImageARGB32(void *buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};
		static const Profile profile;
	};
	// xRGB32
	struct ImageXRGB32 : public Image {
		ImageXRGB32(void *buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};
		static const Profile profile;
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
