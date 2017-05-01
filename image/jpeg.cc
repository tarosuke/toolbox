#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <toolbox/image/jpeg.h>



static jmp_buf env;
static void HandleError(j_common_ptr cinfo){
	longjmp(env, 0);
}


IMAGE* JPEG::New(int fd, J_COLOR_SPACE colorSpace){
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

	//ファイルの頭をチェック
	if(jpeg[0] != 0xff || jpeg[1] != 0xd8 || jpeg[2] != 0xff || jpeg[3] != 0xe0){
		munmap(jpeg, stat.st_size);
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

	try{
		//エラーリターン設定
		if(setjmp(env)){
			throw 0;
		}

		//エラーハンドラ設定
		jerr.error_exit = HandleError;

		//ヘッダ取得
		jpeg_read_header(&ci, TRUE);
		ci.out_color_space = colorSpace; //そして色空間設定

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
}

