/** evdevクラス
 * スレッドを起こして/dev/input/event*を全部開いて入力を待つ
 * グラブするかはコンストラクタ引数次第
 * 入力があったら種別ごとのハンドラ処理させ、後で読めるようにしておく
 * またハンドラは仮想関数なので継承すると即時イベントを取得できる
 */
#include "evdev.h"

#include "../directory/directory.h"

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


namespace wO{

	void Evdev::Thread(){

		fd_set rfds;
		FD_ZERO(&rfds);
		int maxfd(0);

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

			if(select(maxfd, &fds, NULL, NULL, NULL) < 0){
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
					case EV_REL :
					case EV_ABS :
					default :
						break;
					}
				}
			}

		}

	}

}
