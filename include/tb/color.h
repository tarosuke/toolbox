/*
 * Copyright (C) 2025 tarosuke<webmaster@tarosuke.net>
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

	struct Color {
		// 特定色フォーマット
		struct Format {
			enum Index {
				ARGB8888,
				XRGB0888,
				ARGB1555,
				RGB565,
				Grayscale8,
				BW,
			};

			static const Format& Select(Index i) {
				return *formats[(unsigned)i];
			};

			virtual void Post(
				void* left, unsigned x, const Color& color) const = 0;
			virtual Color Pick(const void* left, unsigned x) const = 0;

			const unsigned bpp;
			const unsigned bytesPerPixel;
			const bool isTransparent;
			unsigned Stride(unsigned width) const {
				return (width * bpp + 7) & ~7U;
			};

		protected:
			static const Format* const formats[];
			Format(unsigned bpp, bool isTransparent)
				: bpp(bpp),
				  bytesPerPixel((bpp + 7) / 8),
				  isTransparent(isTransparent) {};
			Format() = delete;
			void* Pixel(void* left, unsigned x) const {
				return (void*)((tb::u8*)left + x * bytesPerPixel);
			};
			const void* Pixel(const void* left, unsigned x) const {
				return (const void*)((const tb::u8*)left + x * bytesPerPixel);
			};
		};

		Color() = default;
		Color(const Color&) = default;
		explicit Color(tb::u32 webColor)
			: Color(FM(webColor >> 24, 255U),
				  FM(webColor >> 16, 255U),
				  FM(webColor >> 8, 255U),
				  FM(webColor, 255U)) {};
		explicit Color(float a, float r, float g, float b) : e{b, g, r, a} {};

		// float operator-(const Color& t) const {
		// 	const float r[4]{
		// 		e[0] - t.e[0], e[1] - t.e[1], e[2] - t.e[2], e[3] - t.e[3]};
		// 	return r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3];
		// };

		Color Learp(const Color t, float ratio) {
			const float rr(1 - ratio);
			return Color(e[0] * rr + t.e[0] * ratio, e[1] * rr + t.e[1] * ratio,
				e[2] * rr + t.e[2] * ratio, e[3] * rr + t.e[3] * ratio);
		};

		// 値の取得
		float A() const { return e[3]; };
		float R() const { return e[2]; };
		float G() const { return e[1]; };
		float B() const { return e[0]; };

		tb::u32 WebColor() const {
			return (U(A(), 255) << 24) | (U(R(), 255) << 16) |
				   (U(G(), 255) << 8) | U(B(), 255);
		};

		float Brightness() const { return (R() + G() + B()) / 3; };

		// 設定
		void A(float v) { e[3] = v; };
		void R(float v) { e[2] = v; };
		void G(float v) { e[1] = v; };
		void B(float v) { e[0] = v; };

		void WebColor(tb::u32 v) {
			A(FM(v >> 24, 255U));
			R(FM(v >> 16, 255U));
			G(FM(v >> 8, 255U));
			B(FM(v, 255U));
		};

	private:
		float e[4];

		template <typename T> static T U(float v, T max) {
			return (T)(v * max);
		};
		template <typename T> static float F(T o, T max) {
			return ((float)o) / max;
		};
		template <typename T> static float FM(T o, T max) {
			return F(o & max, max);
		};
	};
}
