/*********************************************************************** prefs
 * Copyright (C) 2015-2021,2023,2024 tarosuke<webmaster@tarosuke.net>
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
 * ファイルのパスにはレベルごとに以下のものが使われる。ただし0は読み書きされない
 * 1: .
 * 2: ~/.config
 * 3: /etc
 *
 * 値の読み出しは変数のように行えるが、書き込みにはSetを使う。
 * 代入演算子もあるがこれで設定するとlevelに0を指定するのと同じになる。
 *
 * 末端の設定に何かさせたい時はPrefsの子クラスを作ってExtendedHandlerをオー
 * バーライドした上で呼び出す。また全ての設定を辿る時はItorを作って0になるま
 * でループする。
 */
#pragma once

#include <filesystem>
#include <stdlib.h>
#include <tb/string.h>
#include <type_traits>

namespace tb {

	/**
	 * テンプレートでstaticメンバを作っても型ごとにインスタンスができてしまう
	 * のでインスタンスを共通化するための親クラス
	 */
	class CommonPrefs {
		CommonPrefs() = delete;
		CommonPrefs(const CommonPrefs&) = delete;
		void operator=(const CommonPrefs&) = delete;

	public:
		enum class Level : unsigned { value = 0, current, home, etc, nLevel };

		// 属性値
		enum Attribute {
			save,
			nosave,
		};

		// 設定の読み込みと保存のキー
		class Keeper {
			Keeper() = delete;
			Keeper(const Keeper&) = delete;
			void operator=(const Keeper&) = delete;

		public:
			Keeper(int argc, const char** argv, const char* name = 0) {
				Load(argc, argv, name);
				parsed = Parse(argc, argv);
			};
			~Keeper() { CommonPrefs::Store(); };
			void Store() { CommonPrefs::Store(); };
			operator unsigned() { return parsed; };

		private:
			unsigned parsed;
		};

	protected:
		CommonPrefs(const char* key,
			const char* description = 0,
			Attribute attr = save);
		virtual ~CommonPrefs() {};

		virtual String Serialize(Level level) = 0;
		virtual void DeSerialize(unsigned level, const char*) = 0;
		virtual bool IsDirty(unsigned) = 0;

		// 拡張用ハンドラ
		virtual void ExtendedHandler(void* = 0) {};

	private:
		static std::filesystem::path pathes[(unsigned)Level::nLevel];

		static CommonPrefs* q;
		CommonPrefs* next;

		const char* const key;
		const Attribute attr;

		static unsigned Parse(int argc, const char** argv);
		static void Load(int argc, const char** argv, const char* name);
		static void Load(unsigned level);
		static void Store();
		static void Store(const std::filesystem::path&);
		static void LoadLine(unsigned, const std::string&);

		const char* MatchedBody(unsigned level, const char*);
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
			: CommonPrefs(key, description, attr) {};
		Prefs(const char* key,
			const T& defaultValue,
			const char* description = 0,
			Attribute attr = save)
			: CommonPrefs(key, description, attr) {
			Set((unsigned)Level::value, defaultValue);
		};
		~Prefs() {};

		operator const T&() const { return GetValid(); };
		void Set(unsigned level, const T& v) { values[level].Set(v); };
		void operator=(const T& v) { Set(0, v); };
		const T& Get(unsigned level) { return values[level].Get(); };

	protected:
		struct Value {
			Value() : valid(false), dirty(false) {};
			void Set(const T& v) {
				value = v;
				valid = dirty = true;
			};
			String Serialize() {
				String r;
				r.Append(value);
				return r;
			};
			void DeSerialize(const char* t) {
				if constexpr (std::is_same<T, bool>::value) {
					Set(!(*t == 'f' || *t == 'F' || *t == 'n' || *t == 'n' ||
							*t == '0'),
						false);
				} else if constexpr (std::is_integral<T>::value) {
					if constexpr (std::is_unsigned<T>::value) {
						Set(strtoul(t, 0, 10));
					} else {
						Set(strtol(t, 0, 10));
					}
				} else if constexpr (std::is_floating_point<T>::value) {
					Set(atof(t));
				} else if (std::is_same<T, std::string>::value) {
					value = t;
					valid = dirty = true;
				}
			};
			bool valid;
			bool dirty;
			T value;
		} values[(unsigned)Level::nLevel];

	private:
		String Serialize(Level level) final {
			return values[(unsigned)level].Serialize();
		};
		void DeSerialize(unsigned level, const char* value) final {
			values[level].DeSerialize(value);
		};
		bool IsDirty(unsigned l) final { return values[l].dirty; };
		const T& GetValid() const {
			for (unsigned n(0); n < (unsigned)Level::nLevel; ++n) {
				const auto& v(values[n]);
				if (v.valid) {
					return v.value;
				}
			}
			return values[0].value;
		};
	};

}
