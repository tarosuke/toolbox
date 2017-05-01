#pragma once

#include <jpeglib.h>

#include "image.h"


class JPEG : public IMAGE{
	friend class IMAGE;
	JPEG();
	JPEG(const JPEG&);
	void operator=(const JPEG&);
public:
	static IMAGE* New(int, J_COLOR_SPACE); //IMAGE::fdをチェックしJPEGファイルなら読み込む
private:
	static IMAGE* New(int fd){ return New(fd, JCS_EXT_BGR); };
};

