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
#include <math.h>
#include <memory.h>
#include <string.h>
#include <tb/image.h>

namespace tb {

	bool operator==(const Color &o, const Color &t) {
		return o.e[0] == t.e[0] && o.e[1] == t.e[1] && o.e[2] == t.e[2] &&
			   o.e[3] == t.e[3];
	};

	const Image::Profile ImageARGB32::profile{
		elements : {{24, 255}, {16, 255}, {8, 255}, {0, 255}},
		bitsPerPixel : 32
	};
	const Image::Profile ImageXRGB32::profile{
		elements : {{0, 0}, {16, 255}, {8, 255}, {0, 255}},
		bitsPerPixel : 32
	};

	Image::Image(void *buffer, const Image &org, unsigned left, unsigned top,
				 unsigned width, unsigned height, unsigned stride)
		: Image(buffer, org.profile, width, height, stride) {
		const u8 *s(org.buffer + org.stride * left +
					top * org.profile.bitsPerPixel / 8);
		char *d((char *)buffer);
		for (unsigned h(0); h < height; ++h, s += org.stride, d += stride) {
			memcpy(d, s, stride);
		}
	}

	Color Image::Get(unsigned x, unsigned y) const {
		return profile.C(Left(y), x);
	}
	Color Image::Get(float x, float y) const {
		float d;
		const float r[2] = {modff(x, &d), modff(y, &d)};
		const u8 *const l[2] = {Left(y), Left(y + 1)};
		return profile.C(l[0], x)
			.Learp(profile.C(l[0], x + 1), r[0])
			.Learp(profile.C(l[1], x).Learp(profile.C(l[1], x + 1), r[0]),
				   r[1]);
	}

}
