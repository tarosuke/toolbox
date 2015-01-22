#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <jpeglib.h>

#include "jpeg.h"



IMAGE* JPEG::New(int fd){
	//ファイルをまるごとメモリへマップ
	struct stat stat;
	fstat(fd, &stat);
	unsigned char* const jpeg(
		(unsigned char*)mmap(NULL,
			stat.st_size,
			PROT_READ,
			MAP_PRIVATE, fd, 0));
	if(!jpeg){
		return 0;
	}

	IMAGE* image(0);
	struct jpeg_decompress_struct ci;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY jarr(0);

	//伸張準備
	ci.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&ci);

	//元データを設定
	jpeg_mem_src(&ci, jpeg, stat.st_size);

	//ヘッダ取得
	jpeg_read_header(&ci, TRUE);
	ci.out_color_space = JCS_EXT_BGR; //そして色空間設定

	try{
		//画像確保
		image = new IMAGE(ci.image_width, ci.image_height, 3);
		if(!image){
			throw 0;
		}
		char* const buffer((char*)(*image).WritableBuffer());
		const unsigned bpl(ci.image_width * 3);

		//行配列割り当て
		jarr = new JSAMPROW[ci.image_width];
		if(!jarr){
			throw 0;
		}
		for(unsigned n(0); n < ci.image_height; ++n){
			jarr[n] = (JSAMPROW)&buffer[bpl * n];
		}

		//展開開始
		jpeg_start_decompress(&ci);
		while( ci.output_scanline < ci.output_height ) {
			jpeg_read_scanlines( &ci,
				&jarr[ci.output_scanline],
				ci.output_height - ci.output_scanline);
		}

		//終了
		jpeg_finish_decompress(&ci);
	}
	catch(...){
		if(jarr){
			delete jarr;
		}
		if(image){
			delete image;
		}
	}
	jpeg_destroy_decompress(&ci);
	munmap(jpeg, stat.st_size);
	return image;


#if 0
	//シグネチャチェック
	unsigned char head[8];
	read(fd, head, 8);
	if(png_sig_cmp(head, 0, 8)){
		return 0;
	}

	void** rows(0);
	// png_struct
	png_structp png_ptr(png_create_read_struct(
		PNG_LIBPNG_VER_STRING, NULL, NULL, NULL));
	if(!png_ptr){
		//throw "JPEG:png構造を確保できない";
		return 0;
	}
	png_set_sig_bytes(png_ptr, 8);

	// PNGの情報
	png_infop info_ptr(png_create_info_struct(png_ptr));
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr,NULL,NULL);
		//throw "JPEG:png情報(前)を取得できない";
		return 0;
	}

	IMAGE* image(0);
	try{
		//エラーハンドル設定
		if(setjmp(png_jmpbuf(png_ptr))){
			throw 0;
		}

		//読み込みコールバックの設定
		png_set_read_fn(png_ptr,&fd,PNGRead);

		// 画像情報の取得
		png_uint_32 width;
		png_uint_32 height;
		int bit_depth;
		int color_type;
		int interlace_type;
		int compression_type;
		int filter_type;

		png_read_info(png_ptr,info_ptr);
		png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
			&interlace_type,&compression_type,&filter_type);

		//16/chanelは扱えないので8ビットにして取り出す
		if(16 == bit_depth){
			png_set_strip_16(png_ptr);
		}

		//チャンネル数取得
		const unsigned channels(png_get_channels(png_ptr, info_ptr));

		switch(channels){
		case 3 :
		case 4 :
			break;
		default:
			//throw "JPEG:サポートしていないチャネル数";
			throw 0;
		}

		//画像の割り当て
		image = new IMAGE(width, height, channels);
		if(!image){
			throw 0;
		}
		char* const buffer((char*)(*image).WritableBuffer());

		//各行の先頭を配列に
		const unsigned rowbytes(png_get_rowbytes(png_ptr,info_ptr));
		rows = new void*[height];
		if(!rows){
			//throw "JPEG:rowsの確保に失敗";
			throw 0;
		}
		for(unsigned h(0); h < height; ++h){
			rows[h] = &buffer[h * rowbytes];
		}

		//読み込み
		png_set_bgr(png_ptr);
		png_read_image(png_ptr, (png_bytepp)rows);

		//あとしまつ
		free(rows);
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	}
	catch(...){
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if(rows){
			free(rows);
		}
		//throw "JPEG:読み込みに失敗";
		return 0;
	}
	return image;
#endif
}

