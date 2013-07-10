/***************************************************** joystick input:joystick
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <assert.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

#include "joystick.h"



int JOYSTICK::OpenDevice(){
	//デバイスファイルを準備
	for(int i(0); i < 99; i++){
		char name[32];
		snprintf(name, 32, "/dev/input/js%d", i);
		const int fd(open(name, O_RDWR | O_NONBLOCK));
		if(fd < 0){
			//開けなかった
			continue;
		}

		if(flock(fd, LOCK_EX | LOCK_NB) < 0){
			//使用中
			close(fd);
			continue;
		}

		//この時点でのfdが空いていたジョイスティック
		return fd;
	}
	return -1;
}

JOYSTICK::JOYSTICK() : device(OpenDevice()){}

JOYSTICK::~JOYSTICK(){
	close(device);
}

