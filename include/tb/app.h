/************************************************************************* App
 * Copyright (C) 2021,2023 tarosuke<webmaster@tarosuke.net>
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
 ** アプリケーションのフレームワーク
 * Appを導出して必要に応じてMain, Nameをoverride。
 * Nameをoverrideすると設定ファイル名を変更できる(デフォルトは実行ファイル名)
 * 導出したクラスを適当なところにstaticに確保すると初期設定のあとに
 * Init, Mainが呼ばれ、Mainを終了するとFinallyが呼ばれてから終了する。
 * NOTE:構築子が呼ばれるのはstaticインスタンス構築時なので何もできない
 * NOTE:Appが参照された時点でmainはtb::App側で確保されたものがリンクされる
 */
#pragma once

#include <tb/prefs.h>


extern "C" {
	int main(int, char**);
}

namespace tb {

	struct App {
		friend int ::main(int, char**);
		App(const App&) = delete;
		void operator=(const App&) = delete;

		App() { instance = this; };
		virtual ~App(){};

	protected:
		virtual int Main(uint remainArgs, const char**) { return 0; };
		virtual const char* Name() { return 0; };

	private:
		static App* instance;
		static Prefs<unsigned> logLevel;
	};
}
