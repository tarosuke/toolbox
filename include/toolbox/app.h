/************************************************************************* App
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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
 * Appを導出してMain、必要があればFinallyをoverride
 * 導出したクラスのインスタンスをstaticに作るとmainがセットで付いてきて
 * App::Mainが呼ばれる
 */
#pragma once

#include <toolbox/path.h>
#include <toolbox/prefs.h>



namespace TB {

	class App {
		App(const App&);
		void operator=(const App&);

	public:
		App() { instance = this; };
		virtual ~App(){};
		static int main(int argc, const char* argv[]);

	protected:
		static std::string name;

		virtual int Main() = 0;
		virtual void Finally(){};

	private:
		static App* instance;
		static Prefs<unsigned> logLevel;
	};

	/** Mainの時点でインスタンスを作れない場合
	 * これをインスタンス化するとstaticなT::Mainが呼ばれる
	 */
	template <class T> struct StaticApp : App {
		int Main() final { return T::Main(); };
	};
}
