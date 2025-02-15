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
		friend bool operator==(const Color&, const Color&);

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

			const Format& Select(Index i) { return *formats[(unsigned)i]; };

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
				  bytesPerPixel(bpp / 8),
				  isTransparent(isTransparent) {};
		};

		Color() = default;
		Color(const Color&) = default;
		explicit Color(u32 webColor)
			: Color(u8(webColor >> 24),
				  u8(webColor >> 16),
				  u8(webColor >> 8),
				  u8(webColor)) {};
		explicit Color(float a, float r, float g, float b) : e{a, r, g, b} {};
		template <typename T, typename E>
		explicit Color(T a, T r, T g, T b, E max)
			: e{F(a, max), F(r, max), F(g, max), F(b, max)} {};

		float operator-(const Color& t) const {
			const float r[4]{
				e[0] - t.e[0], e[1] - t.e[1], e[2] - t.e[2], e[3] - t.e[3]};
			return r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3];
		};

		Color Learp(const Color t, float ratio) {
			const float rr(1 - ratio);
			return Color(e[0] * rr + t.e[0] * ratio, e[1] * rr + t.e[1] * ratio,
				e[2] * rr + t.e[2] * ratio, e[3] * rr + t.e[3] * ratio);
		};

		float operator[](unsigned i) const { return e[i]; };
		template <typename T> T U(float v, T max) const {
			return (T)(v * max);
		};
		template <typename T, typename E> static float F(T o, E max) {
			return ((float)o) / max;
		};
		operator float() const { return (e[0] + e[1] + e[2]) / 3; };

	private:
		float e[4];
	};
	bool operator==(const Color&, const Color&);

}
