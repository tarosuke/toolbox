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
	bool operator==(const Color& o, const Color& t) {
		return o.e[0] == t.e[0] && o.e[1] == t.e[1] && o.e[2] == t.e[2] &&
			   o.e[3] == t.e[3];
	}

	template <typename T, typename E> struct ByteFormat : Color::Format {
		struct Element {
			T mask;
			unsigned bit;
		};
		const Element (&elements)[4];

		ByteFormat(const Element (&elements)[4])
			: Format((sizeof(T) + 7) / 8, !!elements[3].mask),
			  elements(elements) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			*(T*)((tb::u8*)left + x * bytesPerPixel) =
				(((T)c.U(c[0], max) << elements[0].bit) & elements[0].mask) |
				(((T)c.U(c[1], max) << elements[1].bit) & elements[1].mask) |
				(((T)c.U(c[2], max) << elements[2].bit) & elements[2].mask) |
				(((T)c.U(c[3], max) << elements[3].bit) & elements[3].mask);
		};
		Color Pick(const void* left, unsigned x) const final {
			const T& t(*(const T*)((const tb::u8*)left + x * bytesPerPixel));
			return Color((T)((t & elements[0].mask) >> elements[0].bit),
				(T)((t & elements[1].mask) >> elements[1].bit),
				(T)((t & elements[2].mask) >> elements[2].bit),
				(T)((t & elements[3].mask) >> elements[3].bit), max);
		};

	private:
		static constexpr E max = ~(E)0;
	};

	template <typename T, typename E> struct GrayFormat : Color::Format {
		GrayFormat() : Format(8, false) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			*(T*)((tb::u8*)left + x * bytesPerPixel) = (T)c.U(c, max);
		};
		Color Pick(const void* left, unsigned x) const final {
			const float c(
				Color::F(*(T*)((tb::u8*)left + x * bytesPerPixel), max));
			return Color(1.0, c, c, c);
		};

	private:
		static constexpr E max = ~(E)0;
	};

	struct BitFormat : Color::Format {
		BitFormat() : Format(1, false) {};

		void Post(void* left, unsigned x, const Color& c) const final {
			tb::u8& t(((tb::u8*)left)[x / 8]);
			const tb::u8 mask(1 << (x & 7));
			if (0.5 < c) {
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


	static const ByteFormat<tb::u32, tb::u8>::Element elements8888[4] = {
		{mask : 0x00ff0000, bit : 16},
		{mask : 0x0000ff00, bit : 8},
		{mask : 0x000000ff, bit : 0},
		{mask : 0xff000000, bit : 24},
	};
	static const ByteFormat<tb::u32, tb::u8> format8888(elements8888);
	static const ByteFormat<tb::u32, tb::u8>::Element elements0888[4] = {
		{mask : 0x00ff0000, bit : 16},
		{mask : 0x0000ff00, bit : 8},
		{mask : 0x000000ff, bit : 0},
		{mask : 0, bit : 0},
	};
	static const ByteFormat<tb::u32, tb::u8> format0888(elements0888);
	static const ByteFormat<tb::u16, tb::u8>::Element elements1555[4] = {
		{mask : 0x7c00, bit : 10},
		{mask : 0x03e0, bit : 5},
		{mask : 0x001f, bit : 0},
		{mask : 0x7000, bit : 15},
	};
	static const ByteFormat<tb::u16, tb::u8> format1555(elements1555);
	static const ByteFormat<tb::u16, tb::u8>::Element elements0565[4] = {
		{mask : 0xf800, bit : 11},
		{mask : 0x07e0, bit : 5},
		{mask : 0x001f, bit : 0},
		{mask : 0, bit : 0},
	};
	static const ByteFormat<tb::u16, tb::u8> format0565(elements0565);

	static const GrayFormat<tb::u8, tb::u8> format8;
	static const BitFormat format1;

	const Color::Format* const Color::Format::formats[] = {&format8888,
		&format0888, &format1555, &format0565, &format8, &format1, 0};
}
