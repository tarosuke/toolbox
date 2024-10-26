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
#include <memory.h>
#include <string.h>
#include <tb/image.h>

namespace tb {

	const Image::Profile ImageARGB32::profile{
		a : {24, 0xff000000},
		r : {16, 0x00ff0000},
		g : {8, 0x0000ff00},
		b : {0, 0x000000ff},
		bitsPerPixel : 32
	};
	const Image::Profile ImageXRGB32::profile{
		a : {0, 0},
		r : {16, 0x00ff0000},
		g : {8, 0x0000ff00},
		b : {0, 0x000000ff},
		bitsPerPixel : 32
	};

	Image::Image(void *buffer, const Image &org, unsigned left, unsigned top,
				 unsigned width, unsigned height, unsigned stride)
		: Image(buffer, org.profile, width, height, stride) {
		const char *s(org.buffer + org.stride * left +
					  top * org.profile.bitsPerPixel / 8);
		char *d((char *)buffer);
		for (unsigned h(0); h < height; ++h, s += org.stride, d += stride) {
			memcpy(d, s, stride);
		}
	}

}
