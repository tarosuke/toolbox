#pragma once

#include "image.h"

#include "../factory/factory.h"


class PNG : public IMAGE{
	PNG();
	PNG(const PNG&);
	void operator=(const PNG&);
public:
	PNG(const char* path);
private:
	static FACTORY<IMAGE> factory;
	static IMAGE* New(); //IMAGE::fdをチェックしPNGファイルならnew
	PNG(int fd){ Setup(fd); }; //ファイルディスクリプタで生成
	void Setup(int fd); //初期化、読み込み
};

