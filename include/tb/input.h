/** input interface
 * Copyright (C) 2012 2024 tarosuke<webmaster@tarosuke.net>
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
#pragma once
/** 使い方
 * Inputクラスを導出して必要なイベントのハンドラをoverrideする。
 * GetInputを呼び出すとハンドラが呼ばれるので処理
 * 初期化引数にtrueを与えるとグラブするが、すべてのeventデバイスを
 * グラブしてしまうので注意
 *
 * 使用するときはデバイスファイルのパーミッションを取得しておくこと *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <poll.h>
#include <tb/time.h>
#include <tb/vector.h>
#include <vector>



namespace tb {

	struct Input {
		Input(const Input&) = delete;
		void operator=(const Input&) = delete;

		enum class Key {
			A = 0x01,
			B = 0x02,
			X = 0x08,
			Y = 0x10,
			LB = 0x40,
			RB = 0x80,
			C = 0x1000,
			L3 = 0x2000,
			R3 = 0x4000,
			select = 0x400,
			start = 0x800,
			liftRight = 0x10,
			upDown = 0x11,
		};


		/***** マウス、スティックなど */
		struct AxisReport {
			static constexpr unsigned nAxis = 0x40;
			AxisReport() : value{}, moved(0){};
			void AddEvent(unsigned axis, int v) {
				if (axis < nAxis) {
					moved |= 1 << axis;
					value[axis] += v;
				}
			};
			void SetEvent(unsigned axis, int v) {
				if (axis < nAxis) {
					moved |= 1 << axis;
					value[axis] = v;
				}
			};
			operator bool() const { return !!moved; };
			int value[nAxis]; // 軸ごとの値
			unsigned moved; // イベントがあったビットが1
		};
		/***** グループごとの各種ボタン */
		struct ButtonReport {
			ButtonReport() : up(0), down(0), state(0){};
			operator bool() const { return up || down; };
			void Clear() { up = down = 0; };
			unsigned up;
			unsigned down;
			unsigned state;
		};

		virtual void GetInput() = 0;

	protected:
		Timestamp timestamp;

		Input() = default;
		virtual ~Input(){};

		virtual void OnKeyDown(const Timestamp&, unsigned key){};
		virtual void OnKeyUp(const Timestamp&, unsigned key){};
		virtual void OnKeyRepeat(const Timestamp&, unsigned key){};

		virtual void OnMouseButton(const Timestamp&, const ButtonReport&){};
		virtual void OnWheel(const Timestamp&, const ButtonReport&){};
		virtual void OnGamepad(const Timestamp&, const ButtonReport&){};

		/***** およそマウスの移動
		 */
		virtual void OnRelMoved(const Timestamp&, const AxisReport&){};

		/***** ゲームコントローラーのスティックとトリガなど
		 * 0:左左右
		 * 1:左上下
		 * 2:左トリガ
		 * 3:右左右
		 * 4:右上下
		 * 5:右トリガ
		 */
		virtual void OnAbsMoved(const Timestamp&, const AxisReport&){};
	};
}
