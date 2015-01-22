#pragma once

#include "image.h"


class JPEG : public IMAGE{
	friend class IMAGE;
	JPEG();
	JPEG(const JPEG&);
	void operator=(const JPEG&);
public:
private:
	static IMAGE* New(int); //IMAGE::fdをチェックしJPEGファイルなら読み込む
};

