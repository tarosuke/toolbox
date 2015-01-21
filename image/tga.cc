#include "tga.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>



TGA::TGA(const void* rawTGA) : IMAGE(
	(*static_cast<const RAW*>(rawTGA)).data,
	(*static_cast<const RAW*>(rawTGA)).width,
	(*static_cast<const RAW*>(rawTGA)).height,
	((*static_cast<const RAW*>(rawTGA)).colorDepth + 7) >> 3){
	RAW& tga(*(RAW*)rawTGA);
	if(~tga.attribute & 0x20){
		//上下逆なので正しい向きの画像を用意する
		char* nb((char*)malloc(Size()));
		const unsigned step(Width() * Depth());
		const char* s((const char*)GetConstPoint(0, Height() - 1));
		char* d(nb);
		for(unsigned n(0); n < Height(); ++n, s -= step, d += step){
			memcpy(d, s, step);
		}
		AssignBuffer(nb);
	}

	Dump(rawTGA);
};


void TGA::Dump(const void* rawTga){
	const RAW& r(*(const RAW*)rawTga);
	printf("IDLen:%d.\n", r.IDLen);
	printf("IsColorMap:%d.\n", r.IsColorMap);
	printf("type:%d.\n", r.type);
	printf("colorMapIndex:%d.\n", r.colorMapIndex);
	printf("colorMapLength:%d.\n", r.colorMapLength);
	printf("colorMapEntrySize:%d.\n", r.colorMapEntrySize);
	printf("xOffset:%d.\n", r.xOffset);
	printf("yOffset:%d.\n", r.yOffset);
	printf("width:%d.\n", r.width);
	printf("height:%d.\n", r.height);
	printf("colorDepth:%d.\n", r.colorDepth);
	printf("attribute:%x.\n", r.attribute);
}


IMAGE* TGA::New(int fd){
	//シグネチャチェック
	if(lseek(fd, -18, SEEK_END) < 0){ return 0; };
	char buff[18];
	if(read(fd, buff, 18) < 0
		|| strncmp(buff, "TRUEVISION-XFILE.", 17)){ return 0; };

	//ヘッダ読み出し
	RAW head;
	lseek(fd, 0, SEEK_SET);
	if(read(fd, &head, sizeof(head)) != sizeof(head)){
		return 0;
	};

	//サイズ計算
	const unsigned bpp((head.colorDepth + 7) >> 3);
	const unsigned size(head.width * head.height * bpp);

	//ケーパビリティチェック
	if(head.IsColorMap || (head.type != 2) || !(bpp == 3 || bpp == 4)){
		return 0;
	}

	//IMAGE確保
	IMAGE* image(new IMAGE(head.width, head.height, bpp));
	if(!image){
		return 0;
	}

	//読み込み
	if(read(fd, (*image).WritableBuffer(), size) != size){
		delete image;
		return 0;
	}

	//方向確認
	if(~head.attribute & 0x20){
		(*image).FlipVertical();
	}

	return image;
}

