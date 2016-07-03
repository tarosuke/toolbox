/** evdevクラス
 * マッチしたするevdevを全部開いてスレッドを起こして入力を待つ
 * pathにあるpatternにマッチするファイルを開く
 * 例：Evdev("/dev/input/by-id/", "3Dconnexion")とすると/dev/input/by-id/内の*3Dconnexion*を開く
 * On〜は仮想関数なので置き換えると
 */
#pragma once

#include <linux/input.h>

#include "../thread/thread.h"
#include "../container/ring.h"


namespace TB{

	class Evdev : public THREAD{
	public:
		Evdev(const char* path, const char* pattern ,bool grab = false);

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

	protected:
		/** イベントハンドラ
		 * 第一引数はデバイスを区別するためのファイルデスクリプタ
		 * 第二引数はイベント
		 */
		virtual void OnKEY(int, const input_event&); //キーボードやマウスのボタンなど
		virtual void OnREL(int, const input_event&); //マウスなどの相対位置
		virtual void OnABS(int, const input_event&); //タブレット、タッチパネルなどの絶対位置

	private:
		wO::Ring<unsigned short, int> keyBuff; //上位がbreak、下位がmark
		ButtonState buttons;
		int rel[REL_CNT];
		int abs[ABS_CNT];

		bool keep;
		 int maxfd;
		fd_set rfds;
		void Thread();
	};

}
