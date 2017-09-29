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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <math.h>
#include <cairo/cairo.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <syslog.h>

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
	Image::Image(const char* fileName){
		//PNGとして読んでみる
		surface = cairo_image_surface_create_from_png(fileName);
		if(surface && cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS){
			//PNG読み込み成功
			return;
		}

		//JPEGとして読み込んでみる
		LoadJPEG(fileName);
		if(surface){
			//成功
			return;
		}

		//失敗したのでダミーのコンテンツ割り当てておく
		surface = cairo_image_surface_create( CAIRO_FORMAT_RGB24, 8, 8);
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

	Image::operator Raw() const{
		return (Raw){
			data : cairo_image_surface_get_data(surface),
			transparent : IsTransparent(),
			stride : (unsigned)cairo_image_surface_get_stride(surface),
			width : (unsigned)cairo_image_surface_get_width(surface),
			height : (unsigned)cairo_image_surface_get_height(surface),
			bpp : GetBPP()};
	}
	Image& Image::operator=(const Image& o){
		if(surface){
			cairo_surface_destroy(surface);
		}
		surface = o.CopySurface();
		return *this;
	}


	//Image情報取得
	unsigned Image::GetWidth() const{
	    return (unsigned)cairo_image_surface_get_width(surface);
	}
	unsigned Image::GetHeight() const{
	    return (unsigned)cairo_image_surface_get_height(surface);
	}
	unsigned Image::GetBPP() const{
		switch(cairo_image_surface_get_format(surface)){
		case CAIRO_FORMAT_ARGB32:
		case CAIRO_FORMAT_RGB24:
			return 4;
		case CAIRO_FORMAT_A8:
			return 1;
		case CAIRO_FORMAT_RGB16_565:
			return 2;
		case CAIRO_FORMAT_RGB30:
			return 4;
		default:
			break;
		}
		return 0;
	}
	bool Image::IsTransparent() const{
		return cairo_image_surface_get_format(surface) ==
			CAIRO_FORMAT_ARGB32;
	}

	/** 画像加工
	 */
	Image& Image::Update(const Image& src, int x, int y){
		Raw dr(*this);
		Raw sr(src);

		const unsigned xo(x < 0 ? -x : 0);
		const unsigned yo(y < 0 ? -y : 0);
		const unsigned width(sr.width - xo);
		const unsigned height(sr.height - yo);

		for(unsigned sy(yo); sy < height; ++sy){
			unsigned* const d(&dr[y + sy][x + xo]);
			unsigned* const s(&sr[sy][xo]);
			memcpy(d, s, width * dr.bpp);
		}
		return *this;
	}
	Image& Image::FlipVertical(){
		Raw r(*this);
		for(unsigned y(0); y < r.height / 2; ++y){
			for(unsigned x(0); x < r.width; ++x){
				auto& s(r[y][x]);
				auto& d(r[r.height - y - 1][x]);

				const unsigned t(s);
				s = d;
				d = t;
			}
		}
		return *this;
	}
	Image& Image::FlipHorizontal(){
		Raw r(*this);
		for(unsigned y(0); y < r.height; ++y){
			for(unsigned x(0); x < r.width / 2; ++x){
				auto& s(r[y][x]);
				auto& d(r[y][r.width - x - 1]);

				const unsigned t(s);
				s = d;
				d = t;
			}
		}
		return *this;
	}
	Image& Image::Rotate180(){
		Raw r(*this);
		for(unsigned y(0); y < r.height / 2; ++y){
			for(unsigned x(0); x < r.width; ++x){
				auto& s(r[y][x]);
				auto& d(r[r.height - y - 1][r.width - x - 1]);

				const unsigned t(s);
				s = d;
				d = t;
			}
		}
		return *this;
	}
	Image& Image::Rotate90(){
		Raw rs(*this);
		Image dst(rs.height, rs.width,IsTransparent());
		Raw rd(dst);
		for(unsigned y(0); y < rs.height; ++y){
			for(unsigned x(0); x < rs.width; ++x){
				rd[x][y] = rs[rs.height - y - 1][x];
			}
		}
		cairo_surface_t* const t(surface);
		surface = dst.surface;
		dst.surface = t;
		return *this;
	}
	Image& Image::Rotate270(){
		Raw rs(*this);
		Image dst(rs.height, rs.width,IsTransparent());
		Raw rd(dst);
		for(unsigned y(0); y < rs.height; ++y){
			for(unsigned x(0); x < rs.width; ++x){
				rd[x][y] = rs[y][rs.width - x - 1];
			}
		}
		cairo_surface_t* const t(surface);
		surface = dst.surface;
		dst.surface = t;
		return *this;
	}


	/***** Pen関連
	 */
	//生成と削除
	Image::Pen::Pen(Image& c) :
		canvas(c),
		gc(cairo_create(c.surface)){
		cairo_set_antialias(gc, CAIRO_ANTIALIAS_GRAY);
		cairo_set_operator(gc, CAIRO_OPERATOR_OVER);
	}
	Image::Pen::~Pen(){
		cairo_surface_flush(canvas.surface);
		Raw raw(canvas);
		Rect<unsigned> cr(
			Vector<unsigned>(0, 0),
			Vector<unsigned>(raw.width, raw.height));
		updated &= cr;
		canvas.OnImageUpdated(Raw(canvas), updated);
		cairo_destroy(gc);
	}
	//更新領域管理
	void Image::Pen::Update(const double up[4]){
		//更新範囲を0以上にしてupdatedへ蓄積
		unsigned l(0 < up[0] ? (int)floor(up[0]) : 0);
		unsigned t(0 < up[1] ? (int)floor(up[1]) : 0);
		unsigned r(0 < up[2] ? (int)ceil(up[2]) : 0);
		unsigned b(0 < up[3] ? (int)ceil(up[3]) : 0);
		Rect<unsigned> u(Vector<unsigned>(l, t), Vector<unsigned>(r, b));
		updated |= u;
	}


	/** 色の定義
	 */
	Image::Pen::Color::Color(Image::Color c) : enable(true){
			transparent = !(c & 0xff000000);
			r = ((double)((c >> 16) & 0xff)) / 255.0;
			g = ((double)((c >> 8) & 0xff)) / 255.0;
			b = ((double)(c & 0xff)) / 255.0;
			a = ((double)((c >> 24) & 0xff)) / 255.0;
	}
	Image::Pen::Color::Color(double r, double g, double b) :
		enable(true), transparent(false), r(r), g(g), b(b){}
	Image::Pen::Color::Color(double r, double g, double b, double a) :
		enable(true), transparent(true), r(r), g(g), b(b), a(a){}
	bool Image::Pen::Color::Setup(Pen& gc) const{
		if(enable){
			if(transparent){
				cairo_set_source_rgba(gc, r, g, b, a);
			}else{
				cairo_set_source_rgb(gc, r, g, b);
			}
		}
		return enable;
	}

	/** 文字関連
	 */
	 Image::Pen& Image::Pen::SetFont(
		 const char* name,
		 double size,
		 double slant,
		 double weight){
		 cairo_select_font_face(gc, name,
 			CAIRO_FONT_SLANT_NORMAL,
 			CAIRO_FONT_WEIGHT_NORMAL);
			cairo_set_font_size(gc, size);
		 return *this;
	 }
	 Image::Pen& Image::Pen::Puts(const char* str){
		if(!cairo_has_current_point(gc)){
			syslog(LOG_WARNING, "Canvas:No current point.\n");
		}else{
			cairo_text_path(gc, str);
		}
		return *this;
	 }

	/** 描画条件設定
	 */
	Image::Pen& Image::Pen::operator=(double w){
		cairo_set_line_width(gc, w);
		return *this;
	}
	Image::Pen& Image::Pen::operator=(Cap c){ return *this; }
	Image::Pen& Image::Pen::operator=(Join j){ return *this; }


	/** パス形成
	 */
	 void Image::Pen::MoveTo(double x, double y){
 		cairo_move_to(gc, x, y); };
 	void Image::Pen::LineTo(double x, double y){
 		cairo_line_to(gc, x, y); };
 	void Image::Pen::Rectangle(double x, double y, double w, double h) {
 		cairo_rectangle(gc, x, y, w, h); };
 	void Image::Pen::Arc(double x, double y, double r, double a1, double a2) {
 		cairo_arc(gc, x, y, r, a1, a2); };
 	void Image::Pen::CurveTo(
		double x1, double y1, double x2, double y2, double nx, double ny){
 		cairo_curve_to(gc, x1, y1, x2, y2, nx, ny); };

	/** 描画
	 * ストローク、フィルとして設定された色で描画する
	 * ただし「色」に描画なしが指定されていた場合、塗らない
	 */
	void Image::Pen::Draw(){
		if(fColor.Setup(*this)){
			double p[4];
	 		cairo_fill_extents(gc, &p[0], &p[1], &p[2], &p[3]);
			Update(p);
			if(sColor.enable){
				cairo_fill_preserve(gc);
			}else{
				cairo_fill(gc);
			}
		}
		if(sColor.Setup(*this)){
			double p[4];
	 		cairo_stroke_extents(gc, &p[0], &p[1], &p[2], &p[3]);
			Update(p);
			cairo_stroke(gc);
		}
	}
	//指定色でcanvasを塗る
	void Image::Pen::Clear(const FillColor& c){
		c.Setup(*this);
		cairo_paint(gc);
		const double r[4] = {
			0.0, 0.0,
			(const double)canvas.GetWidth(), (const double)canvas.GetHeight() };
		Update(r);
	}

	/** JPEG対応
	 */
	static jmp_buf env;
	static void HandleError(j_common_ptr cinfo){
		longjmp(env, 0);
	}

	void Image::LoadJPEG(const char* path){
		//ファイルを開いておく
		const int fd(open(path, O_RDONLY));
		if(fd < 0){ return; }; //失敗

		//ファイルをまるごとメモリへマップ
		struct stat stat;
		fstat(fd, &stat);
		unsigned char* const jpeg(
			(unsigned char*)mmap(NULL,
			stat.st_size,
			PROT_READ,
			MAP_PRIVATE, fd, 0));
		if(!jpeg){
			return; //mmap失敗
		}

#if 0
		//ファイルの頭をチェック
		if(jpeg[0] != 0xff || jpeg[1] != 0xd8 || jpeg[2] != 0xff || jpeg[3] != 0xe0){
			munmap(jpeg, stat.st_size);
			return;
		}
#endif

		struct jpeg_decompress_struct ci;
		struct jpeg_error_mgr jerr;
		JSAMPARRAY jarr(0);

		//伸張準備
		ci.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&ci);

		//元データを設定
		jpeg_mem_src(&ci, jpeg, stat.st_size);

		try{
			//エラーリターン設定
			if(setjmp(env)){
				throw 0;
			}

			//エラーハンドラ設定
			jerr.error_exit = HandleError;

			//ヘッダ取得
			jpeg_read_header(&ci, TRUE);
			ci.out_color_space = JCS_EXT_BGRX; //そして色空間設定

			//画像確保
			surface = cairo_image_surface_create(
				CAIRO_FORMAT_RGB24, ci.image_width, ci.image_height);
			if(!surface){
				return; //格納先確保失敗
			}

			//行配列割り当て
			const unsigned bpl(ci.image_width * 4);
			unsigned char* const buff(cairo_image_surface_get_data(surface));
			jarr = new JSAMPROW[ci.image_height];
			if(!jarr){
				throw 0;
			}
			for(unsigned n(0); n < ci.image_height; ++n){
				jarr[n] = (JSAMPROW)&buff[bpl * n];
			}

			//展開開始
			jpeg_start_decompress(&ci);
			while( ci.output_scanline < ci.output_height ) {
				jpeg_read_scanlines(&ci,
					&jarr[ci.output_scanline],
					ci.output_height - ci.output_scanline);
			}

			//終了
			jpeg_finish_decompress(&ci);
		}
		catch(...){
			if(surface){
				cairo_surface_destroy(surface);
				surface = 0;
			}
		}
		jpeg_destroy_decompress(&ci);
		if(jarr){
			delete jarr;
		}
		munmap(jpeg, stat.st_size);
	}

}
