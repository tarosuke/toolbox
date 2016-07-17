/** evdevクラス
 * スレッドを起こして/dev/input/event*を全部開いて入力を待つ
 * グラブするかはコンストラクタ引数次第
 * 入力があったら種別ごとのハンドラ処理させ、後で読めるようにしておく
 * またハンドラは仮想関数なので継承すると即時イベントを取得できる
 */
#include <toolbox/input/evdev.h>

#include <toolbox/directory/directory.h>

#include <sys/select.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>



namespace TB{

	Evdev::Evdev(const char* dirName, const char* pattern, bool grab) : keep(false){
		const char* patterns[] = { pattern, 0 };
		Open(dirName, patterns, grab);
	}
	Evdev::Evdev(const char* dirName, const char** patterns, bool grab) : keep(false){
		Open(dirName, patterns, grab);
	}
	void Evdev::Open(const char* dirName, const char** patterns, bool grab){
		buttons.Reset();

		//selectの準備
		FD_ZERO(&rfds);
		maxfd = 0;

		//ファイルを開いていく
		Directory dir(dirName);
		for(Directory::ITOR i(dir); i; ++i){
			if(i.IsDir()){ continue; } //ディレクトリは対象外だし追いかけない

			for(const char** p(patterns); *p; ++p){
				const char* pattern(*p);

				if(!strstr(i.Name(), pattern)){ continue; } //ファイル名がマッチしなかった

				//パターンがマッチしたので開く
				char path[256];
				snprintf(path, sizeof(path) , "%s/%s", dirName, i.Name());
				const int fd(open(path, O_RDWR));
				if(fd < 0){
					//開けなかった
					continue;
				}

				//ロック
				if(!grab && flock(fd, LOCK_EX | LOCK_NB) < 0){
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

	void Evdev::ThreadBody(){
		keep = true;

		//何か読めたら解釈...の繰り返し
		while(keep){
			fd_set fds(rfds);

			if(select(maxfd + 1, &fds, NULL, NULL, NULL) < 0){
				syslog(LOG_ERR, "Evdev:select gets error");
				return;
			}

			for(int n(0); n <= maxfd; ++n){
				if(FD_ISSET(n, &fds)){
					input_event ev;
					if(read(n, &ev, sizeof(ev)) < 0){
						//読み込みエラーなのでそのevdevは閉じる
						syslog(LOG_ERR, "Evdev:read gets error");
						FD_CLR(n, &rfds);
						close(n);
						continue;
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

	void Evdev::OnKEY(int fd, const input_event& ev){
		if(ev.code < 256){
			//キーボード
			keyBuff = ev.value ? ev.code : ev.code << 8;
		}
		switch(ev.code & 0xfff0){
		case BTN_MOUSE:
			if(ev.value){
				buttons.On(ev.code - BTN_MOUSE);
			}else{
				buttons.Off(ev.code - BTN_MOUSE);
			}
			break;
		case BTN_GAMEPAD:
			if(ev.value){
				buttons.On(ev.code - BTN_GAMEPAD);
			}else{
				buttons.Off(ev.code - BTN_GAMEPAD);
			}
			break;
		}
	}
	void Evdev::OnREL(int, const input_event& ev){
		if(ev.code < REL_CNT){
			rel[ev.code] += ev.value;
		}
	}
	void Evdev::OnABS(int, const input_event& ev){
		if(ev.code < ABS_CNT){
			abs[ev.code] = ev.value;
		}
	}

}
