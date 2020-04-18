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

#include "include/canvas.h"

#include <cairo/cairo.h>

#include <string.h>



namespace TB{

	Canvas::Color::Color(float r, float g, float b, float a) :
			color(
				((static_cast<unsigned>(255.0 * r) & 255U) << 16) |
				((static_cast<unsigned>(255.0 * g) & 255U) << 8) |
				(static_cast<unsigned>(255.0 * b) & 255U) |
				((static_cast<unsigned>(255.0 * a) & 255U) << 24)){}


	Canvas::Raw::Raw(Canvas& canvas) :
			surface(canvas.surface),
			body((unsigned*)cairo_image_surface_get_data(surface)),
			bpp(GetBPP(surface)),
			stride(cairo_image_surface_get_stride(surface) / bpp),
			width(cairo_image_surface_get_width(surface)),
			height(cairo_image_surface_get_height(surface)){
		cairo_surface_flush(surface);
	}

	Canvas::Raw::~Raw(){
		cairo_surface_mark_dirty(surface);
	}

	unsigned Canvas::Raw::GetBPP(cairo_surface_t* surface){
		switch(cairo_image_surface_get_format(surface)){
		case CAIRO_FORMAT_ARGB32:
		case CAIRO_FORMAT_RGB24:
		case CAIRO_FORMAT_RGB30:
			return 4;
		case CAIRO_FORMAT_RGB16_565:
			return 2;
		case CAIRO_FORMAT_A8:
			return 1;
		default:
			break;
		}
		return 0;
	}


	Canvas::Canvas(unsigned width, unsigned height) :
			surface(cairo_image_surface_create(
				CAIRO_FORMAT_ARGB32,
				width,
				height)){}

	Canvas::Canvas(const char* path) : surface(Load(path)){}

	cairo_surface_t* Canvas::Load(const char* path){
		static const struct EXTHandler{
			const char* const ext;
			cairo_surface_t* (* const loader)(const char*);
		}exts[] = {
			{ "png", PNG_Loader },
			{ "ping", PNG_Loader },
			{}
		};

		auto const ext(GetExt(path));
		for(const EXTHandler* l(exts) ; (*l).ext;  ++l){
			if(!strcmp((*l).ext, ext)){
				return (*l).loader(path);
			}
		}
		return 0;
	}

	const char* Canvas::GetExt(const char* p){
		const char* e(0);
		while(*p){
			if(*p++ == '.'){
				e = p;
			}
		}
		return e;
	}

	cairo_surface_t* Canvas::PNG_Loader(const char* path){
		return cairo_image_surface_create_from_png(path);
	}


}
