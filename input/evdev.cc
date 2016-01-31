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


namespace wO{

	Evdev::Evdev(bool grab) : keep(true), maxfd(0){
		FD_ZERO(&rfds);

		//ファイルを開いていく
		Directory dir("/dev/input/by-path");
		for(Directory::ITOR i(dir); i; ++i){
			if(i.IsDir()){ continue; } //ディレクトリは対象外だし追いかけない
			const char* const n(i.Name());
			if(!strstr("-event-mouse", n) ||
				strstr("-event-kbd", n) ||
				strstr("-event-js", n)){

				//eventが取得すべき対象なので開く
				const int fd(open(n, O_RDWR));
				if(fd < 0){
					//開けなかった
					continue;
				}

				//ロック
				if(grab && flock(fd, LOCK_EX | LOCK_NB) < 0){
					//使用中
					close(fd);
					continue;
				}

				//rdfsの設定
				FD_SET(fd, &rfds);
				if(maxfd < fd){
					//maxfd更新
					maxfd = fd;
				}
			}
		}
	}

	void Evdev::Thread(){
		//何か読めたら解釈...の繰り返し
		while(keep){
			fd_set fds(rfds);

			if(select(maxfd, &fds, NULL, NULL, NULL) < 0){
				//selectがエラー
				break;
			}

			for(int n(0); n < maxfd; ++n){
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
						if(!OnKEY(n, ev)){
							Evdev::OnKEY(n, ev);
						}
						break;
					case EV_REL :
						if(!OnREL(n, ev)){
							Evdev::OnREL(n, ev);
						}
						break;
					case EV_ABS :
						if(!OnABS(n, ev)){
							Evdev::OnABS(n, ev);
						}
						break;
					default :
						break;
					}
				}
			}
		}
	}

	bool Evdev::OnKEY(int, const input_event& ev){
		if(ev.code < 256){
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
