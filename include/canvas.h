/********************************************************************** Canvas
 * Copyright (C) 2020 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/geometry/rect.h>



extern "C"{
	typedef struct _cairo cairo_t;
	typedef struct _cairo_surface cairo_surface_t;
}


namespace TB{

	class Canvas{
	public:
		class Color{
		public:
			Color(unsigned webColor=0){ color = webColor; };
			Color(float r=0.0, float g=0.0, float b=0.0, float a=1.0);
		private:
			unsigned color;
		};

		//直接アクセスするためのアクセサ
		class Raw{
			private:
				cairo_surface_t* const surface;
		public:
			Raw(Canvas&);
			~Raw();
			unsigned* const body;
			const unsigned bpp;
			const unsigned stride;
			const unsigned width;
			const unsigned height;
			unsigned* operator[](unsigned y){
				return &body[y * stride];
			};
		private:
			static unsigned GetBPP(cairo_surface_t*);
		};

		Canvas(unsigned width, unsigned height);
		Canvas(const char* path);

	protected:
		cairo_surface_t* const surface;

		virtual void OnUpdated(const Rect<unsigned>&){};

	private:
		static cairo_surface_t* Load(const char*);
		static const char* GetExt(const char*);
		static cairo_surface_t* PNG_Loader(const char*);

	};

}
