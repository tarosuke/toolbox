/** 画像クラス
 * Copyright (C) 2017, 2024 tarosuke<webmaster@tarosuke.net>
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
#include <functional>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <tb/image.h>


namespace tb {

	Image::Image(void* buffer,
		const Image& org,
		unsigned left,
		unsigned top,
		unsigned width,
		unsigned height,
		unsigned stride)
		: Image(buffer, org.format, width, height, stride) {
		const u8* s(org.buffer + org.stride * left + top * org.format.bpp / 8);
		char* d((char*)buffer);
		for (unsigned h(0); h < height; ++h, s += org.stride, d += stride) {
			memcpy(d, s, stride);
		}
	}

	Color Image::Get(unsigned x, unsigned y) const {
		return format.Pick(Left(y), x);
	}

	void Image::Set(unsigned x, unsigned y, const Color& c) {
		format.Post(Left(y), x, c);
	}

	Color Image::Get(float x, float y) const {
		float d;
		const float r[2] = {modff(x, &d), modff(y, &d)};
		const u8* const l[2] = {Left(y), Left(y + 1)};
		return format.Pick(l[0], x)
			.Learp(format.Pick(l[0], x + 1), r[0])
			.Learp(format.Pick(l[1], x).Learp(format.Pick(l[1], x + 1), r[0]),
				r[1]);
	}



	BufferedImage::BufferedImage(
		const Color::Format& format, unsigned width, unsigned height)
		: Image(MkSpec(format, width, height)) {}
	BufferedImage::~BufferedImage() {
		if (buffer) {
			free(buffer);
		}
	}

	BufferedImage::BufferedImage(
		const Image& o, int l, int t, unsigned w, unsigned h)
		: Image(MkSpec(o.Format(), w, h)) {

		// NOTE:以下4つはo上の座標
		const int sy(0 <= t ? t : -t);
		const int ey(std::min(o.Height(), t + h));
		const int sx(0 < l ? l : -l);
		const int ex(std::min(o.Width(), l + w));

		if (ey <= sy || ex <= sx) {
			return; // 範囲なし
		}

		const int th(ey - sy);
		const int offset(sx * format.bpp);
		const int len((ex - sx) * format.bpp);

		for (int y(0 <= t ? 0 : -t); y < th; ++y) {
			memcpy(Left(y), o.Left(t + y) + offset, len);
		}
	}

	Image::Spec BufferedImage::MkSpec(
		const Color::Format& f, unsigned w, unsigned h) {
		Spec spec = {
			format : f,
			width : w,
			height : h,
			stride : f.Stride(w),
		};
		spec.buffer = malloc(spec.stride * h);
		return spec;
	};

}
