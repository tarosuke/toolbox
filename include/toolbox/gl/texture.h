/********************************************************************* TEXTURE
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 */
#pragma once


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
