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
 *
 * 設定はKeeperを作ると読み込まれ、Keeperが消滅すると保存される。
 *
 * 末端の設定に何かさせたい時はPrefsの子クラスを作ってExtendedHandlerをオー
 * バーライドした上で呼び出す。また全ての設定を辿る時はItorを作って0になるま
 * でループする。
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
		// 属性値
		enum Attribute {
			save,
			nosave,
		};

		virtual void Reset() = 0; // 初期値に戻す

		// 設定の読み込みと保存キー
		struct Keeper {
			Keeper(const char* name, int argc = 0, const char** argv = 0) {
				PrefsBase::LoadAll(argc, argv);
			};
			~Keeper() { PrefsBase::StoreAll(); };
			void Store() { PrefsBase::StoreAll(); };

			Keeper() = delete;
			Keeper(const Keeper&) = delete;
			void operator=(const Keeper&) = delete;
		};

	protected:
		const char* const key;
		const Attribute attr;

		PrefsBase(const char* key, Attribute attr);
		~PrefsBase(){};

		virtual void Load(const char* value) = 0;
		virtual void Store(FILE*) = 0;

	private:
		static Path path;
		static PrefsBase* q;

		static void StoreAll();
		static void LoadAll(
			int argc, const char* argv[]); // 設定を読んでコマンドラインで上書き
		static void SetValue(char* line);

		PrefsBase* next;
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
	template <typename T> struct Prefs : public PrefsBase {
		Prefs(const char* key, Attribute attr = save) : PrefsBase(key, attr){};
		Prefs(const char* key, const T& defaultValue, Attribute attr = save)
			: PrefsBase(key, attr), body(defaultValue),
			  defaultValue(defaultValue){};
		~Prefs(){};

		T operator=(const T& v) {
			body = v;
			return body;
		};
		operator const T&() { return body; };

	protected:
		T body;
		T defaultValue;
		void Load(const char* value) override;
		void Store(FILE*) override;
		void Reset() override { body = defaultValue; };
	};

	template <> struct Prefs<char*> : public PrefsBase, public std::string {
		Prefs(const char* key, Attribute attr = save)
			: PrefsBase(key, attr), defaultValue(0){};
		Prefs(const char* key, const char* defaultValue, Attribute attr = save)
			: PrefsBase(key, attr), std::string(defaultValue),
			  defaultValue(defaultValue){};
		~Prefs(){};

	protected:
		const char* const defaultValue;
		void Load(const char* value) override;
		void Store(FILE*) override;
	};

	template <> struct Prefs<std::string> : public PrefsBase,
											public std::string {
		Prefs(const char* key, Attribute attr = save)
			: PrefsBase(key, attr), defaultValue(0){};
		Prefs(const char* key, const char* defaultValue, Attribute attr = save)
			: PrefsBase(key, attr), std::string(defaultValue),
			  defaultValue(defaultValue){};
		~Prefs(){};

	protected:
		const char* const defaultValue;
		void Load(const char* value) override;
		void Store(FILE*) override;
	};
}
