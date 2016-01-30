/** evdevクラス
 * スレッドを起こして/dev/input/event*を全部開いて入力を待つ
 * グラブするかはコンストラクタ引数次第
 * 入力があったら種別ごとのハンドラ処理させ、後で読めるようにしておく
 * またハンドラは仮想関数なので継承すると即時イベントを取得できる
 */
#pragma once

#include <linux/input.h>

#include "../thread/thread.h"

namespace wO{

	class Evdev : public THREAD{
	public:
		Evdev(bool);

		/** 各種ハンドラ
		 * 第一引数はデバイスを区別するためのファイルデスクリプタ
		 * 第二引数はイベント
		 */
		virtual void OnKEY(int, input_event); //キーボード、特殊なキーボードっぽいもの
		virtual void OnBTN(int, input_event); //マウスボタン、ジョイスティックのボタンなど
		virtual void OnREL(int, input_event); //マウスなどの相対位置
		virtual void OnABS(int, input_event); //タブレット、タッチパネルなどの絶対位置

	private:
		bool keep;
		void Thread();
	};

}
