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

#include "geometry/rect.h"



extern "C"{
	typedef struct _cairo cairo_t;
	typedef struct _cairo_surface cairo_surface_t;
}


namespace TB{

	class Canvas{
		Canvas();
		Canvas(const Canvas&);
		void operator=(const Canvas&);
	public:
		class Color{
		public:
			Color(unsigned webColor=0);
			Color(float r, float g, float b, float a=1.0);

			void SetColor(cairo_t*);

		private:
			double r;
			double g;
			double b;
			double a;
		};

		//直接アクセスするためのアクセサ
		class Raw{
			Raw();
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

		//グラフィックコンテンツ
		class GC{
			GC();
			GC(const GC&);
			void operator=(const GC&);
		public:
			GC(Canvas&);
			~GC();

		private:
			cairo_t* gc;
			Canvas& canvas;
			Color strokeColor;
			Color fillColor;
			Rect<double> extents;

			void Flush();
		};

		Canvas(unsigned width, unsigned height);
		Canvas(const char* path);
		virtual ~Canvas();

	protected:
		cairo_surface_t* const surface;

		virtual void OnCanvasUpdated(const Rect<double>&){};

	private:
		static cairo_surface_t* Load(const char*);
		static const char* GetExt(const char*);
		static cairo_surface_t* LoadPNG(const char*);
		static cairo_surface_t* LoadJPEG(const char*);

	};

}
