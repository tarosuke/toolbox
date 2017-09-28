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
 *
 * protectedかpublicで継承してOnUpdatedをオーバーライドして使う。
 * Penを作って描画してPenが消滅する時にOnUpdatedが呼ばれる。
 */
#pragma once
#include <toolbox/geometry/rect.h>



extern "C"{
	typedef struct _cairo cairo_t;
	typedef struct _cairo_surface cairo_surface_t;
}

namespace TB{
	class Image{
		Image();
	public:
		using Color = unsigned;

		class Raw{
		public:
			unsigned char* data;
			bool transparent;
			unsigned stride;
			unsigned width;
			unsigned height;
			unsigned bpp;
			Color Get(float x, float y) const; //補間付きで画素を読む(はみ出たら端の色)
			Color Get(int x, int y) const; //保管なしで画素を読む(はみ出たら端の色)
			bool Set(int x, int y, Color color); //書き込むl(はみ出てたら偽が返る)
			unsigned* operator[](unsigned y) const{ //配列形式でアクセス
				return (unsigned*)&data[y * stride];
			};
		};

		Image(unsigned width, unsigned height, bool transparent);
		Image(const Image&);
		Image(const Image&, int x, int y, unsigned width, unsigned height);
		Image(const char* path);
		~Image();

		operator Raw() const;
		Image& operator=(const Image&);

		unsigned GetWidth() const;
		unsigned GetHeight() const;
		unsigned GetBPP() const;
		operator Rect<unsigned>() const{
			return Rect<unsigned>(
				Vector<unsigned, 2>( 0, 0 ),
				Vector<unsigned, 2>( GetWidth(), GetHeight() )
			);
		};

		/** 画像操作
		 * NOTE:Penが存在するときは使わないこと
		 */
		Image& Update(const Image&, int x, int y); //x,yの位置からImageを書き込む
		Image& FlipVertical();
		Image& FlipHorizontal();
		Image& Rotate180();
		Image& Rotate90();
		Image& Rotate270();
		Image& Rotate(float angle, unsigned backColor); //回転して空きをbackColorで

		/** いわゆるGC
		 */
		class Pen{
			Pen(const Pen&);
			void operator=(const Pen&);
		public:
			Pen(Image&);
			~Pen();


			/** 設定用定義
			 * Penのインスタンスに代入すると設定される
			 */
			enum Cap{ //線端
				buttCap, //そのまま終端
				roundCap, //円
				squareCap //四角
			};
			enum Join{
				miterJoin, //角
				miterRound, //円
				bevelJoin //面取り
			};
			//色指定
			class Color{
			public:
				bool Setup(Pen&) const; //enableを返す
				bool enable;
			public:
				Color(::TB::Image::Color);
				Color(double r, double g, double b);
				Color(double r, double g, double b, double alpha);
			protected:
				Color() : enable(false){};
				bool transparent;
				double r, g, b, a;
			};
			class StrokeColor : public Color{
			public:
				StrokeColor(){};
				StrokeColor(::TB::Image::Color c) : Color(c){};
				StrokeColor(double r, double g, double b) : Color(r,g,b){};
				StrokeColor(double r, double g, double b, double a) : Color(r,g,b,a){};
			};
			class FillColor : public Color{
			public:
				FillColor(){};
				FillColor(::TB::Image::Color c) : Color(c){};
				FillColor(double r, double g, double b) : Color(r,g,b){};
				FillColor(double r, double g, double b, double a) : Color(r,g,b,a){};
			};


			/** 描画メソッド
			 */
			void Draw();
			void Clear(const FillColor&);
			// 属性設定
			Pen& operator=(const StrokeColor& c){ sColor = c; return *this; };
			Pen& operator=(const FillColor& c){ fColor = c; return *this; };
			Pen& operator=(double); //線幅設定
			Pen& operator=(Cap);
			Pen& operator=(Join);
			Pen& SetStrokeColor(){
				*this = StrokeColor();
				return *this;
			}
			Pen& SetStrokeColor(::TB::Image::Color c){
				*this = StrokeColor(c);
				return *this;
			};
			Pen& SetStrokeColor(double r, double g, double b){
				*this = StrokeColor(r, g, b);
				return *this;
			};
			Pen& SetStrokeColor(double r, double g, double b, double a){
				*this = StrokeColor(r, g, b, a);
				return *this;
			};
			Pen& SetFillColor(){
				*this = FillColor();
				return *this;
			}
			Pen& SetFillColor(::TB::Image::Color c){
				*this = FillColor(c);
				return *this;
			};
			Pen& SetFillColor(double r, double g, double b){
				*this = FillColor(r, g, b);
				return *this;
			};
			Pen& SetFillColor(double r, double g, double b, double a){
				*this = FillColor(r, g, b, a);
				return *this;
			};
			Pen& SetFont(
				const char* name,
				double size,
				double slant = 0,
				double weight = 0);

			// パス設定
			void MoveTo(double x, double y);
			void LineTo(double x, double y);
			void Rectangle(double x, double y, double width, double height);
			void Arc(double x, double y, double r, double a1, double a2);
			void CurveTo(double x1, double y1, double x2, double y2, double nx, double ny);
			//文字描画
			Pen& Puts(const char*); //エンコードはUTF8

			/** 生cairo_t取得
			 * NOTE:Penが消滅したあとは無効になる
			 */
			operator cairo_t*(){ return gc; };

		private:
			Image& canvas;
			cairo_t* const gc;
			Rect<unsigned> updated;
			StrokeColor sColor;
			FillColor fColor;
			void Update(const double[4]);
		};

	protected:
		virtual void OnImageUpdated(const Raw&, const Rect<unsigned>&){};
		bool IsTransparent() const;

	private:
		cairo_surface_t* surface;
		cairo_surface_t* CopySurface() const;
		static cairo_surface_t* NewSurface(
			unsigned width,
			unsigned height,
			bool transparent);

		void LoadJPEG(const char* path);

	};
}
