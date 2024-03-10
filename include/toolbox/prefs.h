/*********************************************************************** prefs
 * Copyright (C) 2015-2021 tarosuke<webmaster@tarosuke.net>
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
 * 設定はsystem, user, workplace, temporaryの四種類あり、以下の違いがある。
 * 使うときの優先度はtemporary, workplace, user, systemの順になる。
 *    system:/etc/(プロセス名)に書いてあるものを読む。保存はしない。
 *      user:~/.(プロセス名)を読み書き。
 * workplace:指定したパス/.(プロセス名)を読み書き
 * temporary:ファイルは扱わない。コマンドライン引数向け。
 *
 * また何も設定されていなければコード上で指定されるデフォルト値が使われる
 */
#pragma once

#include <toolbox/path.h>



namespace TB {

	/**
	 * テンプレートでstaticメンバを作っても型ごとにインスタンスができてしまう
	 * のでインスタンスを共通化するための親クラス
	 */
	class PrefsBase {
		PrefsBase();
		PrefsBase(const PrefsBase&);
		void operator=(const PrefsBase&);

	public:
		// 読み出しと保存
		static void Store();
		static void Load(
			int argc,
			const char* argv[]); // 設定を読んでコマンドラインで上書き

	protected:
		const char* const key;

		PrefsBase(const char* key);
		~PrefsBase(){};

		virtual void Read(unsigned index, const char*) = 0;
		virtual void Write(unsigned index, FILE*) = 0;

	private:
		static Path paths[3]; // system, user, workplaceの３つ
		static PrefsBase* q;
		static const char* writeModes[3];

		static void SetValue(unsigned index, char* line);

		PrefsBase* next;

		static void Store(FILE*);
		static void Load(FILE*);
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
	template <typename T> struct Prefs : PrefsBase {
		struct Body {
			bool valid;
			bool dirty;
			T body;

			Body() : valid(false), dirty(false){};
			void operator=(const T& v) {
				body = v;
				valid = true;
				dirty = true;
			};
			void Read(const char*); // NOTE:validを真にすること
			void Write(const char*, FILE*);

			Body(const Body&) = delete;
			void operator=(const Body&) = delete;
		} & system, &user, &current, &temporary;

		Prefs(const char* key)
			: PrefsBase(key), system(bodies[0]), user(bodies[1]),
			  current(bodies[2]), temporary(bodies[3]){};
		Prefs(const char* key, const T& defaultValue)
			: PrefsBase(key), system(bodies[0]), user(bodies[1]),
			  current(bodies[2]), temporary(bodies[3]),
			  defaultValue(defaultValue){};
		~Prefs(){};

		void operator=(const T& v) { temporary = v; };
		operator const T&() {
			return temporary.valid ? temporary.body
				 : current.valid   ? current.body
				 : user.valid	   ? user.body
				 : system.valid	   ? system.body
								   : defaultValue;
		};

	protected:
		T defaultValue;
		Body bodies[4];
		void Read(unsigned index, const char* v) final {
			bodies[index].Read(v);
		};
		void Write(unsigned index, FILE* f) final {
			bodies[index].Write(key, f);
		};
	};
}
