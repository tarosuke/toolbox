/** linux input subsystem
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
#include <vector>



namespace tb {

	class Input {
		Input(const Input&);
		void operator=(const Input&);

	public:
		void GetInput();

	protected:
		Input(msec outTime = 0, bool grab = false);
		virtual ~Input();

		virtual void OnKeyDown(unsigned key){};
		virtual void OnKeyUp(unsigned key){};
		virtual void OnKeyRepeat(unsigned key){};

		virtual void OnButtonDown(unsigned button){};
		virtual void OnButtonUp(unsigned button){};
		virtual void OnMouseMove(unsigned axis, int diff){};

	private:
		static const int relDirs[];
		const int outms;
		std::vector<pollfd> evs;
	};
}
