/***** 画像の直接操作インターフェイス
 * Copyright (C) 2023,2025 tarosuke<webmaster@tarosuke.net>
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
#include <tb/color.h>
#include <tb/rect.h>
#include <tb/spread.h>
#include <tb/types.h>



namespace tb {

	/***** 画像インターフェイス
	 * 既存の画像データを取り出して扱うための抽象クラス
	 */
	struct Image {
		Image() = delete;
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		virtual ~Image() {};

		// 生データ
		const Color::Format& Format() const { return format; };
		void* Data() const { return (void*)buffer; };
		unsigned Width() const { return width; };
		unsigned Height() const { return height; };
		bool Transparent() const { return !!format.isTransparent; }
		tb::Spread<2, int> Spread() const {
			return tb::Spread<2, int>((int[]){(int)width, (int)height});
		};
		u8* Left(unsigned y) const { return buffer + (y % height) + stride; };

		Color Get(unsigned x, unsigned y) const;
		void Set(unsigned x, unsigned y, const Color&);
		Color Get(float x, float y) const;




	protected:
		u8* const buffer;
		const Color::Format& format;
		const unsigned width;  // [px]
		const unsigned height; // [px]
		const unsigned stride; // [bytes]

		/***** Imageインターフェイスの構築子
		 * 特クラスから貰った諸元を記録するだけ
		 */
		Image(void* buffer,
			const Color::Format& format,
			unsigned width,
			unsigned height,
			unsigned stride)
			: buffer((u8*)buffer),
			  format(format),
			  width(width),
			  height(height),
			  stride(stride) {};

		/***** 与えられたImageの一部を複製
		 */
		Image(void* buffer,
			const Image& org,
			unsigned left,
			unsigned top,
			unsigned width,
			unsigned height,
			unsigned stride);

		/***** 構造体から構築
		 */
		struct Spec {
			void* buffer;
			const Color::Format& format;
			unsigned width;
			unsigned height;
			unsigned stride;
		};
		Image(const Spec& s)
			: Image(s.buffer, s.format, s.width, s.height, s.stride) {};
	};

	/***** バッファ自動管理Image
	 * 普通のImageはすでにあるImageを扱うことを意図しているが、BufferedImageは
	 * バッファ自体も管理することを想定している
	 */
	struct BufferedImage : public Image {
		BufferedImage(const Color::Format&, unsigned width, unsigned height);
		BufferedImage(const Image& o, int x, int y, unsigned w, unsigned h);
		~BufferedImage();

	private:
		static Spec MkSpec(const Color::Format& f, unsigned w, unsigned h);
	};
}
