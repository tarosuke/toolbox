/** input interface
 * Copyright (C) 20212 2024 tarosuke<webmaster@tarosuke.net>
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

		static constexpr unsigned nAxis = 8;

	public:
		virtual void GetInput() = 0;

	protected:
		Input() = default;
		virtual ~Input(){};

		virtual void OnKeyDown(unsigned key){};
		virtual void OnKeyUp(unsigned key){};
		virtual void OnKeyRepeat(unsigned key){};

		virtual void OnMouseDown(unsigned button){};
		virtual void OnMouseUp(unsigned button){};

		virtual void OnWheel(const tb::Vector<2, float>&){};

		/***** マウス、スティックなど */
		struct AxisReport {
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

		/***** およそマウスの移動
		 */
		virtual void OnRelMoved(const AxisReport&){};

		/***** ゲームコントローラーのスティックとトリガなど
		 * 0:左左右
		 * 1:左上下
		 * 2:左トリガ
		 * 3:右左右
		 * 4:右上下
		 * 5:右トリガ
		 */
		virtual void OnAbsMoved(const AxisReport&){};
	};
}
