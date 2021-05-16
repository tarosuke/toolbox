/***************************************************** joystick input:joystick
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <linux/joystick.h>
#include <assert.h>

#include <toolbox/input/joystick.h>



int JOYSTICK::OpenDevice(){
	//デバイスファイルを準備
	for(int i(0); i < 99; i++){
		char name[32];
		snprintf(name, 32, "/dev/input/js%d", i);
		const int fd(open(name, O_RDWR));
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

JOYSTICK::JOYSTICK() : device(OpenDevice()){
	//データ取得開始
	pthread_create(&sensorThread, NULL, _JSThread, (void*)this);
}

JOYSTICK::~JOYSTICK(){
	if(0 <= device){
		pthread_cancel(sensorThread);
		close(device);
	}
}

void* JOYSTICK::_JSThread(void* js_){
	(*(JOYSTICK*)js_).JSThread();
	return 0;
}

void JOYSTICK::JSThread(){
	for(;; pthread_testcancel()){
		struct js_event ev;
		if(8 == read(device, &ev, sizeof(ev))){
			switch(ev.type & ~JS_EVENT_INIT){
			case JS_EVENT_AXIS :
				if(ev.number < maxAxis){
					axis[ev.number] = ev.value;
				}
				break;
			case JS_EVENT_BUTTON :
				if(ev.value){
					buttons |= 1U << ev.number;
				}else{
					buttons &= ~(1U << ev.number);
				}
				break;
			}
		}
	}
}

