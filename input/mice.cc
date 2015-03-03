#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <linux/input.h>
#include <assert.h>

#include "mice.h"


void MICE::Thread(){
	if(file < 0){
		puts("devicefile could not open.\nTry to join input group or chmod to read /dev/input/mice.");
		return;
	}

	PS2 pack;
	while(3 == read(file, &pack, sizeof(pack))){
		const Report report = { pack.buttons,
			{	pack.Xmovement,
				pack.Ymovement }};
		reporter(report);
	}
}

int MICE::OpenFile(){
	return open("/dev/input/mice", O_RDONLY);
}
