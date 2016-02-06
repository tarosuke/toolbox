/** evdevクラス
 * スレッドを起こして/dev/input/event*を全部開いて入力を待つ
 * グラブするかはコンストラクタ引数次第
 * 入力があったら種別ごとのハンドラ処理させ、後で読めるようにしておく
 * またハンドラは仮想関数なので継承すると即時イベントを取得できる
 */
#include "evdev.h"

#include "../directory/directory.h"

#include <sys/select.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

//TODO:あとで消す
#include "../../wOLIB/debug.h"


namespace wO{

	bool Evdev::keep(false);
	int Evdev::maxfd(0);
	fd_set Evdev::rfds;

	void Evdev::Thread(){
		if(keep){
			//起動済み
			return;
		}
		keep = true;

		FD_ZERO(&rfds);

		//ファイルを開いていく
		static const char* const  dirName = "/dev/input/by-path";
		Directory dir(dirName);
		for(Directory::ITOR i(dir); i; ++i){
			if(i.IsDir()){ continue; } //ディレクトリは対象外だし追いかけない
			const char* const n(i.Name());
			if(strstr(n, "-event-mouse") ||
				strstr(n, "-event-kbd") ||
				strstr(n, "-event-js")){

				//eventが取得すべき対象なので開く
				char path[256];
			snprintf(path, sizeof(path) , "%s/%s", dirName, n);
			const int fd(open(path, O_RDWR));
			if(fd < 0){
				//開けなかった
				continue;
			}

			//ロック
// 			if(grab && flock(fd, LOCK_EX | LOCK_NB) < 0){
// 				//使用中
// 				close(fd);
// 				continue;
// 			}
DPRINTF("evdev open:%s(%d).\n", path, fd);
			//rdfsの設定
			FD_SET(fd, &rfds);
			if(maxfd < fd){
				//maxfd更新
				maxfd = fd;
			}
				}
		}

		//何か読めたら解釈...の繰り返し
		while(keep){
			fd_set fds(rfds);

			if(select(maxfd + 1, &fds, NULL, NULL, NULL) < 0){
				//selectがエラー
				break;
			}

			for(int n(0); n <= maxfd; ++n){
				if(FD_ISSET(n, &fds)){
					input_event ev;
					if(read(n, &ev, sizeof(ev)) < 0){
						//読み込みエラーなのでそのevdevは閉じる
						FD_CLR(n, &rfds);
						close(n);
					}

					//読めたevを解釈
					switch(ev.type){
					case EV_KEY :
						OnKEY(n, ev);
						break;
					case EV_REL :
						OnREL(n, ev);
						break;
					case EV_ABS :
						OnABS(n, ev);
						break;
					default :
						break;
					}
				}
			}
		}
	}

	bool Evdev::OnKEY(int fd, const input_event& ev){
		if(ev.code < 256){
DPRINTF("ev.code:%x(%d).\n", ev.code, fd);
			//キーボード
			keyBuff = ev.value ? ev.code : ev.code << 8;
			return true;
		}
		switch(ev.code & 0xfff0){
		case BTN_MOUSE:
			if(ev.value){
				mButtons.On(ev.code - BTN_MOUSE);
			}else{
				mButtons.Off(ev.code - BTN_MOUSE);
			}
			break;
		case BTN_GAMEPAD:
			if(ev.value){
				gpButtons.On(ev.code - BTN_GAMEPAD);
			}else{
				gpButtons.Off(ev.code - BTN_GAMEPAD);
			}
			break;
		}
		return true;
	}
	bool Evdev::OnREL(int, const input_event& ev){
		if(ev.code < REL_CNT){
			rel[ev.code] += ev.value;
		}
		return true;
	}
	bool Evdev::OnABS(int, const input_event& ev){
		if(ev.code < ABS_CNT){
			abs[ev.code] = ev.value;
		}
		return true;
	}

}
