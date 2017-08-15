#pragma once

#include "../thread/pthread.h"

class MICE : public TB::PThread{
	MICE();
	MICE(const MICE&);
	void operator=(const MICE&);
public:
	struct Report{
		unsigned buttons;
		int axis[2];
	};

	MICE(void (*reporter)(const Report&)) : enable(true),file(OpenFile()), reporter(reporter){
		RaiseThread();
	}
	void Quit(){ enable = false; };
private:
	bool enable;

	const int file;
	static int OpenFile();
	void (*const reporter)(const Report&);

	struct PS2{
		unsigned char buttons : 3;
		unsigned char : 1;
		unsigned char Y_sign : 1;
		unsigned char X_sign : 1;
		unsigned char Y_overFlow : 1;
		unsigned char X_overFlow : 1;
		char Xmovement;
		char Ymovement;
	};

	void ThreadBody() final;
};

