/*********************************************************************** prefs
 * Copyright (C) 2015-2021,2023 tarosuke<webmaster@tarosuke.net>
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
 ** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと変数を設定値として管理して
 * くれる便利クラス
 *
 *
 * 設定はKeeperを作ると読み込まれ、Keeperが消滅すると保存される。
 * Keeperにはargc,argv,nameを与える。nameが省略されるとargv[0]の最後が使われる。
 *
 * 末端の設定に何かさせたい時はPrefsの子クラスを作ってExtendedHandlerをオー
 * バーライドした上で呼び出す。また全ての設定を辿る時はItorを作って0になるま
 * でループする。
 */
#pragma once

#include <tb/path.h>
#include <tb/string.h>



namespace tb {

	/**
	 * テンプレートでstaticメンバを作っても型ごとにインスタンスができてしまう
	 * のでインスタンスを共通化するための親クラス
	 */
	class CommonPrefs {
		CommonPrefs();
		CommonPrefs(const CommonPrefs&);
		void operator=(const CommonPrefs&);

	public:
		// 属性値
		enum Attribute {
			save,
			nosave,
		};

		// 設定の読み込みと保存キー
		class Keeper {
			Keeper();
			Keeper(const Keeper&);
			void operator=(const Keeper&);

		public:
			Keeper(int argc, const char** argv, const char* name = 0) {
				Parse(argc, argv);
				Load(argc, argv, name);
			};
			~Keeper() { CommonPrefs::Store(); };
			void Store() { CommonPrefs::Store(); };
		};

	protected:
		CommonPrefs(
			const char* key,
			const char* description = 0,
			Attribute attr = save);
		virtual ~CommonPrefs(){};

		virtual String Serialize() = 0;
		virtual void DeSerialize(const char*) = 0;

		// 拡張用ハンドラ
		virtual void ExtendedHandler(void* = 0){};

	private:
		static Path path;

		static CommonPrefs* q;
		CommonPrefs* next;

		const char* const key;
		const Attribute attr;

		static void Parse(int argc, const char** argv);
		static void Load(int argc, const char** argv, const char* name);
		static void Store();
		static void LoadLine(const char*);

		const char* MatchedBody(const char*);
	};

	/**
	 * 型とキー文字列を与えてインスタンスを作るだけで設定になる変数を作れる
	 * クラス
	 * NOTE:keyは文字列リテラルである必要がある
	 * NOTE:「=」で代入できる必要がある
	 * NOTE:operatorによる変換は用意してはあるが、代入以外はキャストする必要が
	 * ある
	 * NOTE:処理がmainに入るまでは機能しない
	 */
	template <typename T> class Prefs : public CommonPrefs {
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);

	public:
		Prefs(
			const char* key, const char* description = 0, Attribute attr = save)
			: CommonPrefs(key, description, attr){};
		Prefs(
			const char* key,
			const T& defaultValue,
			const char* description = 0,
			Attribute attr = save)
			: CommonPrefs(key, description, attr), body(defaultValue){};
		~Prefs(){};

		operator const T&() const { return body; };
		void operator=(const T& v) { body = v; };

	protected:
		T body;

	private:
		String Serialize() final;
		void DeSerialize(const String&) final;
	};
}
