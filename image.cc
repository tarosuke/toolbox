/** 画像クラス
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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
#include <cairo/cairo.h>

#include <toolbox/image.h>

namespace TB{

	/** コンストラクタとデストラクタ
	 */
	Image::Image(unsigned width, unsigned height, bool transparent) :
		surface(NewSurface(width, height, transparent)){};
	Image::Image(const Image& o) : surface(o.CopySurface()){}
	Image::Image(const Image& o, int x, int y, unsigned width, unsigned height) :
		surface(NewSurface(width, height, o.IsTransparent())){
		//oの中身を転送
		const Raw raw(o);
		const unsigned char* s(raw.data + raw.stride*y + x*4);
		unsigned char* d(cairo_image_surface_get_data(surface));
		const unsigned stride((unsigned)cairo_image_surface_get_stride(surface));
		for(unsigned n(0); n < height; ++n, s += raw.stride, d += stride){
			memcpy(d, s, stride);
		}
	}

	Image::~Image(){
		if(surface){
			cairo_surface_destroy(surface);
		}
	}

	/** 便利メソッド
	 */
	cairo_surface_t* Image::CopySurface() const{
		cairo_format_t format(cairo_image_surface_get_format(surface));
		const unsigned width((unsigned)cairo_image_surface_get_width(surface));
		const unsigned height((unsigned)cairo_image_surface_get_width(surface));
		return cairo_surface_create_similar_image(surface, format, width, height);
	}
	cairo_surface_t* Image::NewSurface(unsigned w, unsigned h, bool t){
		const cairo_format_t format(
			t ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24);
		return cairo_image_surface_create(format, w, h);
	}

	bool Image::IsTransparent() const{
		return cairo_image_surface_get_format(surface) == CAIRO_FORMAT_ARGB32;
	}
	Image::operator Raw() const{
		return (Raw){
			data : cairo_image_surface_get_data(surface),
			transparent : IsTransparent(),
			stride : (unsigned)cairo_image_surface_get_stride(surface),
			width : (unsigned)cairo_image_surface_get_width(surface),
			height : (unsigned)cairo_image_surface_get_height(surface)};
	}
	Image& Image::operator=(const Image& o){
		if(surface){
			cairo_surface_destroy(surface);
		}
		surface = o.CopySurface();
		return *this;
	}

}
