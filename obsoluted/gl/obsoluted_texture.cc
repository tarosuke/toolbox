/********************************************************************* TEXTURE
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include "../../obsoluted/include/gl/texture.h"
#include <toolbox/image/image.h>

namespace GL{

	TEXTURE::BINDER::~BINDER(){
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TEXTURE::BINDER::Set(TEXTURE* t){
		glBindTexture(GL_TEXTURE_2D, t ? (*t).tid : 0);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	}



	const TEXTURE::PARAMS TEXTURE::defaultParams = {
		wrap_s : GL_REPEAT,
		wrap_t : GL_REPEAT,
		filter_mag : GL_LINEAR,
		filter_min : GL_LINEAR,
		texture_mode : GL_REPLACE,
		pointSprite : false,
	};

	TEXTURE::TEXTURE(const PARAMS& p) :
		tid(GetNewTID()), empty(true), cairoTransparent(false){}

	TEXTURE::TEXTURE(
		unsigned w,
		unsigned h,
		Format f,
		const PARAMS& p) :
			tid(GetNewTID()),
			empty(true),
			cairoTransparent(f == cairoRGBA || f == compressedCairoRGBA){
		BINDER b(*this);

		glTexImage2D(
			GL_TEXTURE_2D, 0,
			GLTexFormat(f),
			w, h, 0,
			GLImageFormat(f),
			GL_UNSIGNED_BYTE, 0);

		SetupAttributes(p);
		empty = false;
	}

	TEXTURE::TEXTURE(
		const class IMAGE& image, const PARAMS& p) :
		tid(GetNewTID()), empty(true), cairoTransparent(false){
		Assign(image, p);
	}

	TEXTURE::~TEXTURE(){
		glDeleteTextures(1, &tid);
	}


	void TEXTURE::Assign(const IMAGE& image, const PARAMS& p, bool comp){
		BINDER b(*this);

		const unsigned d(image.Depth());
		glTexImage2D(
			GL_TEXTURE_2D, 0,
			d <= 3 ?
				( comp ? GL_COMPRESSED_RGB : GL_RGB) :
				( comp ? GL_COMPRESSED_RGBA : GL_RGBA),
			image.Width(), image.Height(), 0,
			d <= 3 ? GL_BGR : GL_BGRA,
			GL_UNSIGNED_BYTE, image.Buffer());
		empty = false;

		//属性を設定
		SetupAttributes(p);
	}

	void TEXTURE::SetupAttributes(const PARAMS& p){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p.wrap_t);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,p.filter_mag);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,p.filter_min);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, p.texture_mode);

		if(p.pointSprite){
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}
	}

	void TEXTURE::Update(const IMAGE& image, int x, int y){
		if(empty){
			//テクスチャメモリが割り当てられていないので終了
			return;
		}
		BINDER b(*this);
		glTexSubImage2D(
			GL_TEXTURE_2D, 0,
			x,
			y,
			image.Width(),
			image.Height(),
			image.Depth()==4 ? GL_BGRA : GL_BGR,
			GL_UNSIGNED_BYTE,
			image.Buffer());
	}
	void TEXTURE::Update(
		const void* buffer, Format format, int x, int y, unsigned width, unsigned height){
		if(empty){
			//テクスチャメモリが割り当てられていないので終了
			return;
		}
		BINDER b(*this);
		glTexSubImage2D(
			GL_TEXTURE_2D, 0,
			x,
			y,
			width,
			height,
			GLImageFormat(format),
			GL_UNSIGNED_BYTE,
			buffer);
	}

	void TEXTURE::TexCoord(float u, float v){
		glTexCoord2f(u, v);
	}

	unsigned TEXTURE::GetNewTID(){
		unsigned id;
		glGenTextures(1, &id);
		return id;
	}

	int TEXTURE::GLTexFormat(Format f){
		switch(f){
		case RGB:
		case BGR:
		case cairoRGB:
			return GL_RGB;
		case RGBA:
		case BGRA:
		case cairoRGBA:
			return GL_RGBA;
		case GRAYSCALE:
			return GL_LUMINANCE8;
		case compressedCairoRGB:
			return GL_COMPRESSED_RGB;
		case compressedCairoRGBA:
			return GL_COMPRESSED_RGBA;
		}
		return GL_LUMINANCE8;
	}
	int TEXTURE::GLImageFormat(Format f){
		switch(f){
		case RGB:
			return GL_RGB;
		case BGR:
			return GL_BGR;
		case RGBA:
			return GL_RGBA;
		case cairoRGB:
		case cairoRGBA:
		case BGRA:
		case compressedCairoRGB:
		case compressedCairoRGBA:
			return GL_BGRA;
		case GRAYSCALE:
			return GL_LUMINANCE;
		}
		return GL_RGB;
	}


	Texture::Texture(
		unsigned w,
		unsigned h,
		Format format,
		const PARAMS& p) :
		TEXTURE(Pow2(w), Pow2(h), format, p),
		hRatio((float)w / Pow2(w)),
		vRatio((float)h / Pow2(h)){}

	void Texture::TexCoord(float u, float v){
		glTexCoord2f(u * hRatio, v * vRatio);
	}

	unsigned Texture::Pow2(unsigned v){
		for(unsigned n(0); n < sizeof(v) * 8 - 1; ++n){
			v |= v >> 1;
		}
		return v + 1;
	}

}
