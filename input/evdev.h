/** evdevクラス
 * スレッドを起こして関係するevdevを全部開いて入力を待つ
 * グラブするかはコンストラクタ引数次第
 * 入力があったら種別ごとのハンドラ処理させ、後で読めるようにしておく
 * またハンドラは仮想関数なので継承すると即時イベントを取得できる
 */
#pragma once

#include <linux/input.h>

#include "../thread/thread.h"
#include "../container/ring.h"


namespace wO{

	class Evdev : public THREAD{
	public:
		Evdev(bool = false);

	protected:
		class  ButtonState{
		public:
			unsigned on; //前回読んだ時から押されたもの(読んだらクリアされる)
			unsigned off; //前回読んだ時から放されたもの(読んだらクリアされる)
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

		/** 各種ハンドラ
		 * 第一引数はデバイスを区別するためのファイルデスクリプタ
		 * 第二引数はイベント
		 */
		virtual bool OnKEY(int, const input_event&); //キーボードやマウスのボタンなど
		virtual bool OnREL(int, const input_event&); //マウスなどの相対位置
		virtual bool OnABS(int, const input_event&); //タブレット、タッチパネルなどの絶対位置

		/** 状態読み取り
		 */
		int ReadKey(){ return keyBuff; };
		ButtonState MouseButtons(){
			const ButtonState s(mButtons);
			mButtons.Reset();
			return s;
		};
		ButtonState GamepadButtons(){
			const ButtonState s(mButtons);
			mButtons.Reset();
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
		Ring<unsigned short, int> keyBuff; //上位がbreak、下位がmark
		ButtonState mButtons;
		ButtonState gpButtons;
		int rel[REL_CNT];
		int abs[ABS_CNT];

		bool keep;
		int maxfd;
		fd_set rfds;
		void Thread();
	};

}
