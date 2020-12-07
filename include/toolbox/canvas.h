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

	/** Canvas
	 * @brief 描画先(いわゆるDrawable)
	 */
	class Canvas{
		Canvas();
		Canvas(const Canvas&);
		void operator=(const Canvas&);
	public:
		/** Color
		 * @brief 色
		 */
		class Color{
		public:
			Color() : r(0), g(0),b(0), a(0){};
			Color(unsigned webColor);
			Color(float r, float g, float b, float a=1.0);

			void SetColor(cairo_t*) const;

		private:
			double r;
			double g;
			double b;
			double a;
		};

		/** Raw
		 * @brief イメージに直接アクセスするためのアクセサ
		 */
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

		/** Clip
		 * クリップ
		 * NOTE:Canvasからはみ出た場合など、与えたサイズと取り出したサイズが異なる場合があるので与えたサイズではなく出てきたサイズを使うこと。
		 */
		class Clip : private Raw{
			Clip();
			Clip(Canvas&, int x, int y, unsigned width, unsigned height);
			void operator=(const Clip&);
		public:
			const unsigned width; //幅
			const unsigned height; //高さ
			const unsigned bpp; //ピクセルのバイト数
			const unsigned stride; //行のバイト数
			void* const body; //画像イメージ
			Clip(const Canvas&);
			~Clip();

		private:
			static unsigned Range(int p, unsigned width, unsigned orgWidth);
		};

		/** GC
		 * @brief グラフィックコンテキスト＆描画メソッド
		 */
		class GC{
			GC();
			GC(const GC&);
			void operator=(const GC&);
		public:
			/** Path
			 *  @brief 多角形などのような閉じたパスを作る時にRAIIする
			 */
			class Path{
				Path(const Path&);
				void operator=(const Path&);
			public:
				Path(GC&);
				~Path();

			private:
				cairo_t* const gc;
			};

			enum Slant{ slant_normal, slant_italic, slant_oblique };
			enum Weight{ weight_normal, weight_bold };
			enum Cap{ cap_butt, cap_round, cap_square };
			enum Join{ join_miter, join_round, join_bevel };

			GC(Canvas&);
			~GC();

			//設定
			void SetStroke(const Color&);
			void SetFill(const Color&);
			void Set(const Color& strokeColor, const Color& fillColor);
			void Set(
				const char* family,
				Slant=slant_normal,
				Weight=weight_normal);
			void SetThickness(double);
			void Set(Cap);
			void Set(Join);

			//描画
			void Clear(const Color&);
			void MoveTo(double x, double y);
			void LineTo(double x, double y);
			void Arc(
				double x,
				double y,
				double radius,
				double startAngle,
				double endAngle);
			void CurveTo(
				double x0, double y0,
				double xc, double yc,
				double x1, double y1);
			void Rectandle(
				double x0, double y0,
				double x1, double y1);
			void Puts(const char* utf8);

		private:
			cairo_t* gc;
			Canvas& canvas;
			Color strokeColor;
			Color fillColor;
			double thickness;
			Cap cap;
			Join join;
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
