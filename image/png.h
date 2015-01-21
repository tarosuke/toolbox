#pragma once

#include "image.h"

#include "../factory/factory.h"


class PNG : public IMAGE{
	friend class IMAGE;
	PNG();
	PNG(const PNG&);
	void operator=(const PNG&);
public:
private:
	static IMAGE* New(int); //IMAGE::fdをチェックしPNGファイルなら読み込む
};

