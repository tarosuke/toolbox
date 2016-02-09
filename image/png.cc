#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include <toolbox/image/png.h>



static void PNGRead(png_structp png_ptr,png_bytep data,png_size_t length){
	const int& fd(*(int*)png_get_io_ptr(png_ptr));
	const int result(read(fd, data, length));
	if(result < 0){
		//読み込み失敗
		png_error(png_ptr, "error at read(2)");
	}else if((unsigned)result != length){
		png_warning(png_ptr, "not cobpletely at read(2)");
	}
}


IMAGE* PNG::New(int fd){
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
		//throw "PNG:png構造を確保できない";
		return 0;
	}
	png_set_sig_bytes(png_ptr, 8);

	// PNGの情報
	png_infop info_ptr(png_create_info_struct(png_ptr));
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr,NULL,NULL);
		//throw "PNG:png情報(前)を取得できない";
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
			//throw "PNG:サポートしていないチャネル数";
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
			//throw "PNG:rowsの確保に失敗";
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
		//throw "PNG:読み込みに失敗";
		return 0;
	}
	return image;
}

