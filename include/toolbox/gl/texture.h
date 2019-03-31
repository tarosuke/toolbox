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



namespace TB{

	class Texture{
		Texture();
		Texture(const Texture&);
		void operator=(const Texture&);
	public:

		struct Style{
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
			RGBA,
			BGRA,
			GRAYSCALE,
		};

		class Binder{
			Binder(const Binder&);
			void operator=(const Binder&);
		public:
			Binder(const Texture&);
			~Binder();
		private:
		};

		Texture(
			unsigned width,
			unsigned height,
			Format = RGB,
			const Style& = defaultStyle);
		Texture(
			const void*,
			unsigned width,
			unsigned height,
			Format = RGB,
			const Style& = defaultStyle);

		~Texture();

		void Update(
			const void*,
			int x,
			int y,
			unsigned width,
			unsigned height,
			Format format);

		//
		//フォーマットが透過ならtrue
		//
		static bool IsTransparent(Format);

		//
		// アクセサ
		//
		unsigned TextureID(){ return tid; };

	private:
		static unsigned NewID();
		static int ToGLFormat(Format);
		static void SetupAttributes(const Style&);

		static const Style defaultStyle;

		const unsigned tid;
	};

}
