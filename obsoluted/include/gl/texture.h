/** Texture
 * Copyright (C) 2017,2019 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/lock/key.h>



//NOTE:OBSOLUTED

class IMAGE;
namespace GL{
	class TEXTURE{
// 		TEXTURE();
		TEXTURE(const TEXTURE&);
		void operator=(const TEXTURE&);
	public:
		class BINDER{
			BINDER();
			BINDER(const BINDER&);
			void operator=(const BINDER&);
		public:
			BINDER(TEXTURE& t){ Set(&t); };
			BINDER(TEXTURE* t){ Set(t); };
			~BINDER();
		private:
			static void Set(TEXTURE*);
		};
		struct PARAMS{
			int wrap_s;
			int wrap_t;
			int filter_mag;
			int filter_min;
			int texture_mode;
			bool pointSprite;
		};

		enum Format{
			RGB,
			BGR,
			cairoRGB,
			compressedCairoRGB,
			RGBA,
			BGRA,
			cairoRGBA,
			compressedCairoRGBA,
			GRAYSCALE,
		};

		TEXTURE(const PARAMS& p=defaultParams);
		TEXTURE(unsigned w,
			unsigned h,
			Format format=RGB,
			const PARAMS& p=defaultParams);
		TEXTURE(const IMAGE&, const PARAMS& p=defaultParams);
		~TEXTURE();

		void Assign(
			const IMAGE&,
			const PARAMS& p=defaultParams,
			bool compress=false);
		void Update(const IMAGE&, int x, int y);
		void Update(
			const void*,
			Format format,
			int x, int y,
			unsigned width,
			unsigned height);

		void TexCoord(float u, float v);

	protected:
		static const PARAMS defaultParams;
		unsigned TextureID(){ return tid; }

	private:
		const unsigned tid;
		bool empty;
		const bool cairoTransparent;

		void SetupAttributes(const PARAMS&);
		static unsigned GetNewTID();
		static int GLTexFormat(Format);
		static int GLImageFormat(Format);
	};

	class Texture : public TEXTURE{
		Texture();
		Texture(const Texture&);
		void operator=(const Texture&);
	public:
		Texture(
			unsigned w,
			unsigned h,
			Format format=RGB,
			const PARAMS& p=defaultParams);

		void TexCoord(float u, float v);

	private:
		const float hRatio;
		const float vRatio;
		unsigned Pow2(unsigned);
	};
}
