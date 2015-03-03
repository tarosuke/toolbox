#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <linux/input.h>
#include <assert.h>

#include "mice.h"


void MICE::Thread(){
	if(file < 0 || !reporter){
		return;
	}

	PS2 pack;
	while(3 == read(file, &pack, sizeof(pack))){
		const Report report = { pack.buttons,
			{	pack.X_sign ? -pack.Xmovement : pack.Xmovement,
				pack.Y_sign ? -pack.Ymovement : pack.Ymovement }};
		reporter(report);
	}
}

int MICE::OpenFile(){
	return open("/dev/input/mice", O_RDONLY);
}
