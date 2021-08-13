/************************************************************** riff container
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/type.h>



namespace TB {

	struct Chunk {
		u32 id;
		u32 size;
		char body[0];
		Chunk(u32 id, u32 size) : id(id), size(size){};
	};


#define S2ID(s)                                                                \
	(((u32)((s)[3]) << 24) | ((u32)((s)[2]) << 16) | ((u32)((s)[1]) << 8) |    \
	 (u32)((s)[0]))


	struct RIFF : Chunk {
		enum struct ChunkID : u32 {
			RIFF = S2ID("RIFF"),
			format = S2ID("fmt\0"),
			format = S2ID("data"),
		};

		struct FMT : Chunk {
		private:
			enum FormatTag : u16 {
				PCM = 0x0001,
			} formatTag;
			u16 channels; // おそらく2のみが有効
			u32 samplesPerSec;
			u32 bytesPerSec;
			u16 blockAlign;
			u16 bitsPerSample;
		};

		struct PCM : Chunk {
		private:
			struct Frame {
				i16 left;
				i16 right;
			} data[0];
		};


		RIFF(const char* format);

	private:
		char type[4]; //"wave"
	};


}
