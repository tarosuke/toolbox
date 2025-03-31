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
#include <tb/color.h>


namespace tb {
	template <typename T> struct ByteFormat : Color::Format {
		struct Elements {
			struct Element {
				T mask; // 最大値を兼ねるのでbitでシフトする前の値
				unsigned bit;
			} a, r, g, b;
		};
		const Elements elements;

		ByteFormat(const Elements& elements)
			: Format(sizeof(T) * 8, !!elements.a.mask),
			  elements(elements) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			*(T*)Pixel(left, x) =
				(U(c.A(), elements.a.mask) << elements.a.bit) |
				(U(c.R(), elements.r.mask) << elements.r.bit) |
				(U(c.G(), elements.g.mask) << elements.g.bit) |
				(U(c.B(), elements.b.mask) << elements.b.bit);
		};
		Color Pick(const void* left, unsigned x) const final {
			const T& t(*(const T*)(Pixel(left, x)));
			return Color(F((t >> elements.a.bit), elements.a.mask),
				F((t >> elements.r.bit), elements.r.mask),
				F((t >> elements.g.bit), elements.g.mask),
				F((t >> elements.b.bit), elements.b.mask));
		};

	private:
		static T U(float v, T max) { return (T)(v * max); };
		static float F(T v, T max) { return (float)(v & max) / max; };
	};

	template <typename T, T max> struct GrayFormat : Color::Format {
		GrayFormat() : Format(8, false) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			*(T*)(Pixel(left, x)) = U(c.Brightness());
		};
		Color Pick(const void* left, unsigned x) const final {
			const float c(F(*(const T*)Pixel(left, x)));
			return Color(1.0, c, c, c);
		};

	private:
		static T U(float v) { return (T)(v * max); };
		static float F(T v) { return (float)(v & max) / max; };
	};

	struct BitFormat : Color::Format {
		BitFormat() : Format(1, false) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			tb::u8& t(((tb::u8*)left)[x / 8]);
			const tb::u8 mask(1 << (x & 7));
			if (0.5 < c.Brightness()) {
				t |= mask;
			} else {
				t &= ~mask;
			}
		};
		Color Pick(const void* target, unsigned bit) const final {
			const float c((*(tb::u8*)target & (1 << (bit & 7))) ? 1.0 : 0.0);
			return Color(1.0, c, c, c);
		};
	};


	static const ByteFormat<tb::u32>::Elements elements8888{
		a : {mask : 255, bit : 24},
		r : {mask : 255, bit : 16},
		g : {mask : 255, bit : 8},
		b : {mask : 255, bit : 0},
	};
	static const ByteFormat<tb::u32> format8888(elements8888);
	static const ByteFormat<tb::u32>::Elements elements0888{
		a : {mask : 0, bit : 0},
		r : {mask : 255, bit : 16},
		g : {mask : 255, bit : 8},
		b : {mask : 255, bit : 0},
	};
	static const ByteFormat<tb::u32> format0888(elements0888);
	static const ByteFormat<tb::u16>::Elements elements1555{
		a : {mask : 0x1, bit : 15},
		r : {mask : 0x001f, bit : 10},
		g : {mask : 0x001f, bit : 5},
		b : {mask : 0x001f, bit : 0},
	};
	static const ByteFormat<tb::u16> format1555(elements1555);
	static const ByteFormat<tb::u16>::Elements elements0565{
		a : {mask : 0, bit : 0},
		r : {mask : 0x001f, bit : 11},
		g : {mask : 0x003f, bit : 5},
		b : {mask : 0x001f, bit : 0},
	};
	static const ByteFormat<tb::u16> format0565(elements0565);

	static const GrayFormat<tb::u8, 255> format8;
	static const BitFormat format1;

	const Color::Format* const Color::Format::formats[] = {&format8888,
		&format0888, &format1555, &format0565, &format8, &format1, 0};
}
