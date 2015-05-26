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
			BINDER(const TEXTURE&);
			~BINDER();
		private:
			static unsigned lastBinded;
			const unsigned prevBinded;
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
			RGBA,
			BGRA,
			cairoRGBA,
		};

		TEXTURE(const PARAMS& p=defaultParams);
		TEXTURE(unsigned w,
			unsigned h,
			Format format=RGB,
			const PARAMS& p=defaultParams);
		TEXTURE(const IMAGE&, const PARAMS& p=defaultParams);
		~TEXTURE();

		void Assign(const IMAGE&, const PARAMS& p=defaultParams);
		void Update(const IMAGE&, int x, int y);
		void Update(const void*, Format format, int x, int y, unsigned width, unsigned height);
	private:
		const unsigned tid;
		bool empty;
		void SetupAttributes(const PARAMS&);
		static unsigned GetNewTID();
		static int GLTexFormat(Format);
		static int GLImageFormat(Format);

		static const PARAMS defaultParams;
	};
}

