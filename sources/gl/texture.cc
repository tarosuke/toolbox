/** Texture
 * Copyright (C) 2019 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/gl/gl.h>
#include <toolbox/gl/texture.h>



namespace TB{

	Texture::Texture(
		unsigned width, unsigned height, Format format, const Style& style)
		: tid(NewID()), transparent(IsTransparent(format)) {
		Binder b(*this);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			ToGLFormat(format),
			width,
			height,
			0,
			ToGLFormat(format),
			GL_UNSIGNED_BYTE,
			0);

		SetupAttributes(style);
	}

	Texture::Texture(
		const void* buffer,
		unsigned width,
		unsigned height,
		Format format,
		const Style& style)
		: tid(NewID()), transparent(IsTransparent(format)) {
		Binder b(*this);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			IsTransparent(format) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			buffer);

		SetupAttributes(style);
	}

	Texture::Texture(const Image& image, const Style& style) : tid(NewID()){
		Binder(*this);
		Image::Raw raw(image);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			raw.transparent ? GL_RGBA : GL_RGB,
			raw.width,
			raw.height,
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			raw.data);

		SetupAttributes(style);
		transparent = raw.transparent;
	}

	Texture::~Texture(){
		glDeleteTextures(1, &tid);
	}


	void Texture::Update(
		const void* buffer,
		int x,
		int y,
		unsigned width,
		unsigned height,
		Format format){
		Binder b(*this);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			x,
			y,
			width,
			height,
			ToGLFormat(format),
			GL_UNSIGNED_BYTE,
			buffer);
	}


	unsigned Texture::NewID(){
		unsigned id;
		glGenTextures(1, &id);
		return id;
	}

	int Texture::ToGLFormat(Format format){
		switch(format){
		case RGB:
			return GL_RGB;
		case BGR:
			return GL_BGR;
		case RGBA:
			return GL_RGBA;
		case BGRA:
			return GL_BGRA;
		default:
			break;
		}
		return GL_LUMINANCE8;
	}

	bool Texture::IsTransparent(Format format){
		switch(format){
		case RGBA:
		case BGRA:
			return true;
		default:
			break;
		}
		return false;
	}

	void Texture::SetupAttributes(const Style& style){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, style.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, style.wrap_t);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,style.filter_mag);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,style.filter_min);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, style.texture_mode);

		if(style.pointSprite){
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}
	}

	const Texture::Style Texture::defaultStyle={
		wrap_s : GL_REPEAT,
		wrap_t : GL_REPEAT,
		filter_mag : GL_LINEAR,
		filter_min : GL_LINEAR,
		texture_mode : GL_REPLACE,
		pointSprite : false,
	};



	Texture::Binder::Binder(const Texture& t){
		glBindTexture(GL_TEXTURE_2D, t.tid);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Texture::Binder::~Binder(){
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
