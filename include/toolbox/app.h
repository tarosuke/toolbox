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
 * Appを導出してどこかstaticなところにインスタンスを作る
 *   その時Run、必要があればFinallyをoverride
 *     Ronは初期化後、Finallyはthrowで落ちようとも終了前に呼ばれる
 * コンストラクタの段階では設定も引数も読み込まれていない
 *   なので初期化処理はRunの冒頭で
 */
#pragma once



namespace TB{

	class App{
		App(const App&);
		void operator=(const App&);
	public:
		App();
		virtual ~App(){};

	protected:
		virtual void Init(){};
		virtual bool Run(){ return true; };
		virtual void Finally(){};

	private:
		static App* stack;
		App* next;

		static void InitAll();
		static bool RunAll();
		static void FinallyAll();
	};

}
