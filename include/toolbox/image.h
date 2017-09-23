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
#pragma once
#include <toolbox/geometry/rect.h>



extern "C"{
	typedef struct _cairo_surface cairo_surface_t;
}

namespace TB{
	class Image{
		Image();
	public:
		struct Raw{
			unsigned char* data;
			bool transparent;
			unsigned stride;
			unsigned width;
			unsigned height;
		};

		Image(unsigned width, unsigned height, bool transparent);
		Image(const Image&);
		Image(const Image&, int x, int y, unsigned width, unsigned height);
		~Image();

		operator Raw() const;


		/** いわゆるGC
		 */
		class Pen{
			Pen(const Pen&);
			void operator=(const Pen&);
		public:
			Pen(Image&);
			~Pen();

			/** 描画メソッド
			 */
			void Stroke();
			void Fill();
			void Clear();
			void Paint(){ Clear(); };
			// 属性設定
			void SetColor(unsigned c);
			void SetColor(double r, double g, double b);
			void SetColor(double r, double g, double b, double alpha);
			void SetLineWidth(double);
			// パス設定
			void MoveTo(double x, double y);
			void LineTo(double x, double y);
			void Rectangle(double x, double y, double width, double height);
			void Arc(double x, double y, double r, double a1, double a2);
			void CurveTo(double x1, double y1, double x2, double y2, double nx, double ny);
			//文字設定、描画
			void SelectFont(const char*); //TODO：weightやslantも設定できるようにする
			void SetFontSize(double);
			void Puts(const char*); //エンコードはUTF8

		private:
			Image& canvas;
			cairo_t* const gc;


			void Update(const double[4]);
		};

	protected:
		virtual void OnUpdated(Raw, Rect<double>&);

	private:
		cairo_surface_t* surface;
		Rect<double> updateRect;
		cairo_surface_t* CopySurface() const;
		static cairo_surface_t* NewSurface(
			unsigned width,
			unsigned height,
			bool transparent);

		bool IsTransparent() const;
	};
}
