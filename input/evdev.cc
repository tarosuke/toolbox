/** evdevクラス
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
		RaiseThread();
	}
	Evdev::Evdev(const char* dirName, const char** patterns, bool grab) : keep(false){
		Open(dirName, patterns, grab);
		RaiseThread();
	}
	void Evdev::Open(const char* dirName, const char** patterns, bool grab){
		buttons.Reset();

		//selectの準備
		FD_ZERO(&rfds);
		maxfd = 0;

		//ファイルを開いていく
		Directory dir(dirName);
		for(Directory::Itor i(dir); ++i;){
			if((*i).IsDir()){ continue; } //ディレクトリは対象外だし追いかけない
			for(const char** p(patterns); *p; ++p){
				const char* pattern(*p);

				if(!strstr(*i, pattern)){ continue; } //ファイル名がマッチしなかった

				//パターンがマッチしたので開く
				char path[256];
				snprintf(path, sizeof(path) , "%s/%s", dirName, (const char*)*i);
				const int fd(open(path, O_RDWR));
				if(fd < 0){
					//開けなかった
					syslog(LOG_ERR, "Evdev: failed to open %s", path);
					continue;
				}

				//ロック
				if(!grab && flock(fd, LOCK_EX | LOCK_NB) < 0){
					//使用中
					syslog(LOG_ERR, "Evdev: failed to lock %s", path);
					close(fd);
					continue;
				}

				//使用するデバイス
				syslog(LOG_INFO, "Evdev:%s", path);

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
					//evdevとして開いたものであることを確認
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
