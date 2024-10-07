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
#include <tb/rect.h>
#include <tb/spread.h>
#include <tb/types.h>



namespace tb {

	struct BaseColor {
		struct Profile {
			struct {
				unsigned position;
				unsigned mask;
			} a, r, g, b; // a.maskが0でなければ透過
			unsigned bitsPerPixel;
		};
	};

	/***** 汎用色 */
	template <typename ELEMENT_TYPE> struct Color : public BaseColor {
		Color() = default;
		Color(ELEMENT_TYPE r, ELEMENT_TYPE g, ELEMENT_TYPE b, ELEMENT_TYPE a)
			: a(a), r(r), g(g), b(b) {};

		union {
			u8 raw8[0];
			u16 raw16[0];
			u32 raw32[0];
			struct {
				ELEMENT_TYPE a;
				ELEMENT_TYPE r;
				ELEMENT_TYPE g;
				ELEMENT_TYPE b;
			};
		};

	protected:
		Color(unsigned* d, const unsigned* s, unsigned e) {
			for (unsigned n(0); n < e; ++n, *d++ = *s++) {}
		};
	};

	// TODO:ELEMENT_TYPEがfluatやdoubleのときの扱い

	/***** ARGB32bitの色 */
	struct ARGB32 : public Color<u8> {
		ARGB32() = default;
		ARGB32(u32 c) : Color(raw32, &c, 1) {};
		ARGB32(u8 r, u8 g, u8 b, u8 a) : Color(r, g, b, a) {};
		bool operator==(const ARGB32& t) const {
			return raw32[0] == t.raw32[0];
		};
		static const Profile profile;
	};
	/***** XRGB32bitの色(不透過) */
	struct XRGB32 : public Color<u8> {
		XRGB32() = default;
		XRGB32(u32 c) : Color(raw32, &c, 1) {};
		XRGB32(u8 r, u8 g, u8 b) : Color(r, g, b, 255) {};
		bool operator==(const ARGB32& t) const {
			return raw32[0] == t.raw32[0];
		};
		static const Profile profile;
	};

	/***** 画像インターフェイス
	 * 既存の画像データを取り出して扱うための抽象クラス
	 */
	struct Image {
		Image() = delete;
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		virtual ~Image() {};

		// 生データ
		void* Data() const { return (void*)buffer; };
		unsigned Width() const { return width; };
		unsigned Height() const { return height; };
		bool Transparent() const { return !!profile.a.mask; }
		tb::Spread<2, int> Spread() const {
			return tb::Spread<2, int>((int[]){(int)width, (int)height});
		};

		// 一行分
		struct Line {
			Line() = delete;
			Line(void* left, const BaseColor::Profile& profile, unsigned width)
				: left(left), profile(profile), width(width) {};

			unsigned operator[](unsigned);
			operator void*() { return left; };

		private:
			void* left;
			const BaseColor::Profile& profile;
			const unsigned width; // [px]
		};

		Line operator[](unsigned);

	protected:
		char* const buffer;

		/***** Imageインターフェイスの構築子
		 * 特クラスから貰った諸元を記録するだけ
		 */
		Image(
			void* buffer,
			const BaseColor::Profile& profile,
			unsigned width,
			unsigned height,
			unsigned stride)
			: buffer((char*)buffer), profile(profile), width(width),
			  height(height), stride(stride) {};


		/***** 与えられたImageの一部を複製
		 */
		Image(
			void* buffer,
			const Image& org,
			unsigned left,
			unsigned top,
			unsigned width,
			unsigned height,
			unsigned stride);

	private:
		const BaseColor::Profile& profile;
		const unsigned width; // [px]
		const unsigned height; // [px]
		const unsigned stride; // [bytes]
	};


	// ARGB32
	struct ImageARGB32 : public Image {
		ImageARGB32(void* buffer, unsigned width, unsigned height)
			: Image(buffer, ARGB32::profile, width, height, width * 4) {};
	};
	// xRGB32
	struct ImageXRGB32 : public Image {
		ImageXRGB32(void* buffer, unsigned width, unsigned height)
			: Image(buffer, XRGB32::profile, width, height, width * 4) {};
	};


	/***** バッファ自動管理Image
	 * 普通のImageはすでにあるImageを扱うことを意図しているが、BufferedImageは
	 * バッファ自体も管理することを想定している
	 * NOTE:テンプレート引数TにはImageARGB32のような具象Image型を与える
	 */
	template <class T> struct BufferedImage : public T {
		BufferedImage(unsigned width, unsigned height)
			: T(new tb::u32[width * height], width, height) {};
		BufferedImage(
			const Image& origin,
			tb::Vector<2, int>& offset,
			unsigned width,
			unsigned height)
			: Image(
				  new tb::u32[width * height],
				  origin,
				  offset[0],
				  offset[1],
				  width,
				  height) {};

		~BufferedImage() { delete[] (tb::u32*)Image::buffer; };
	};
}
