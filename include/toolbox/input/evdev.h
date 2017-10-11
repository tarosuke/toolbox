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
 *
 * pathにあるpatternにマッチするファイルを全備開入力を待つ
 * 例：Evdev("/dev/input/by-id/", "3Dconnexion")とすると
 * /dev/input/by-id/内の*3Dconnexion*を開く
 * NOTE:patternsの方を使う時はchar*の配列を0終端すること
 */
#pragma once

#include <linux/input.h>

#include "../thread/pthread.h"
#include "../container/ring.h"


namespace TB{

	class Evdev : public PThread{
		Evdev();
		Evdev(const Evdev&);
		void operator=(const Evdev&);
	public:
		Evdev(const char* path, const char* pattern ,bool grab = false);
		Evdev(const char* path, const char** patterns ,bool grab = false); //複数パターン用

		/** ボタン状態保持
		 */
		class  ButtonState{
		public:
			unsigned on;
			unsigned off;
			unsigned state; //現在の状態

			void On(unsigned bit){
				const unsigned b(1U << (bit & 15));
				on |= b;
				state |= b;
			};
			void Off(unsigned bit){
				const unsigned b(1U << (bit & 15));
				off |= b;
				state &= ~b;
			};
			void Reset(){
				on = off = 0;
			};
		};

		/** 状態読み取り
		 */
		int ReadKey(){ return keyBuff; };
		ButtonState Buttons(){
			const ButtonState s(buttons);
			buttons.Reset();
			return s;
		};
		int GetRel(unsigned n){
			if(n < REL_CNT){
				const int v(rel[n]);
				rel[n] = 0;
				return v;
			}
			return 0;
		};
		int GetAbs(unsigned n){
			return n < ABS_CNT ? abs[n] : 0;
		};

	private:
		/** イベントハンドラ
		 * 第一引数はデバイスを区別するためのファイルデスクリプタ
		 * 第二引数はイベント
		 */
		void OnKEY(int, const input_event&); //キーボードやマウスのボタンなど
		void OnREL(int, const input_event&); //マウスなどの相対位置
		void OnABS(int, const input_event&); //タブレット、タッチパネルなどの絶対位置

		/** 各種バッファ
		 */
		wO::Ring<unsigned short, int, 0> keyBuff; //上位がbreak、下位がmark
		ButtonState buttons;
		int rel[REL_CNT];
		int abs[ABS_CNT];

		/** 実行管理
		 */
		bool keep;
		int maxfd;
		fd_set rfds;
		void ThreadBody() final;

		void Open(const char*, const char**, bool);
	};

}
