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
#include <type_traits>



namespace tb {

	/***** 変換やらのための画素データ
	 */
	template <typename T> struct Pixel {};

	template <> struct Pixel<tb::u8> {
		Pixel(tb::u32 c = 0) : color(c) {};
		Pixel(tb::u8 a, tb::u8 r, tb::u8 g, tb::u8 b)
			: color(
				  ((tb::u32)a << 24) | ((tb::u32)r << 16) | ((tb::u32)g << 8) |
				  (tb::u32)b) {};
		Pixel(const float (&c)[4]) : color(F2u(c)) {};
		Pixel(const tb::u8 (&c)[4]) : color(A2u(c)) {};
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
	 * 既存の画像データを取り出して扱うための抽象クラス
	 */
	struct Image {
		Image() = delete;
		Image(const Image&) = delete;
		void operator=(const Image&) = delete;

		virtual ~Image() {};

		struct Profile {
			struct {
				unsigned position;
				unsigned mask;
			} a, r, g, b; // a.maskが0でなければ透過
			unsigned bitsPerPixel;
		};

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
			Line(void* left, const Profile& profile, unsigned width)
				: left(left), profile(profile), width(width) {};

			unsigned operator[](unsigned);
			operator void*() { return left; };

		private:
			void* left;
			const Profile& profile;
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
			const Profile& profile,
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
		const Profile& profile;
		const unsigned width; // [px]
		const unsigned height; // [px]
		const unsigned stride; // [bytes]
	};


	// ARGB32
	struct ImageARGB32 : public Image {
		ImageARGB32(void* buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};

	private:
		static const Profile profile;
	};
	// xRGB32
	struct ImageXRGB32 : public Image {
		ImageXRGB32(void* buffer, unsigned width, unsigned height)
			: Image(buffer, profile, width, height, width * 4) {};

	private:
		static const Profile profile;
	};


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
