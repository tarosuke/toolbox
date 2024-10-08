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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <tb/prefs.h>
#include <tb/string.h>


namespace tb {

	CommonPrefs* CommonPrefs::q(0);
	Path CommonPrefs::path;


	void CommonPrefs::LoadLine(const char* line) {
		for (CommonPrefs* i(q); i; i = (*i).next) {
			const auto len(strlen(i->key));
			if (!strncmp(i->key, line, len)) {
				// 形式チェック
				if (len < 1 || line[len] != '=') {
					syslog(LOG_ALERT, "invalid option:%s", line);
					return;
				}
				i->DeSerialize(line + len + 1);
			}
		}
	}

	void CommonPrefs::Load(int argc, const char** argv, const char* n) {
		// nが指定されていたらそれを、でなければargv[0]のファイル名を使う
		String name(n ? n : Path::Base(argv[0]));

		// 設定ファイルのパスを生成
		const char* const home(getenv("HOME"));
		path = home ? home : "/root";
		path += "/.";
		path += name;

		FILE* const file(fopen(path.c_str(), "r"));

		if (!file) {
			return;
		}

		for (char b[256]; fgets(b, sizeof(b), file);) {
			auto l(strlen(b));
			if (1 <= l && (b[l - 1] == '\n' || b[l - 1] == '\r')) {
				// 末尾の改行は潰す
				b[l - 1] = 0;
			}
			LoadLine(b);
		}
		fclose(file);
	}

	unsigned CommonPrefs::Parse(int argc, const char** argv) {
		for (int n(1); n < argc; ++n) {
			const char* const arg(argv[n]);
			String line;

			if (arg[0] != '-' && arg[0] != '+') {
				// 解釈できる形式は終了
				return n;
			}

			// キーの取り出し
			if (arg[0] == '-' && arg[1] == '-') {
				if (strchr(arg, '=')) {
					// 値オプション(--xxx=yyy)
					line = arg;
				} else {
					// 値オプション(--xxx yyy)
					if (argc <= ++n) {
						syslog(LOG_CRIT, "option %s needs parametor", arg);
						return n;
					}
					line = arg;
					line += '=';
					line += argv[n];
				}
			} else if ((arg[0] == '+' || arg[0] == '-') && arg[1] != '-') {
				// スイッチオプション(-/+xxx)
				line = "-";
				line += &arg[1];
				line += '=';
				line += arg[0] == '-' ? 'f' : 't';
			}

			LoadLine(line.c_str());
		}
		return argc;
	}

	void CommonPrefs::Store() {
		FILE* const file(fopen(path.c_str(), "w"));

		if (!file) {
			return;
		}

		for (CommonPrefs* i(q); i; i = (*i).next) {
			if (i->attr == save) {
				String line(i->key);
				line += '=';
				line += i->Serialize();
				line += '\n';
				fputs(line.c_str(), file);
			}
		}
		fclose(file);
	}


	CommonPrefs::CommonPrefs(
		const char* key, const char* description, Attribute attr)
		: key(key), attr(attr) {
		// スタックに自身を追加
		next = q;
		q = this;
	}



	// Integerみたいな抽象型がないばっかりにこんなことになっているksg
	template <> String Prefs<bool>::Serialize() {
		return String(body ? "true" : "false");
	}
	template <> String Prefs<i8>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<i16>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<i32>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<i64>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<u8>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<u16>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<u32>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	template <> String Prefs<u64>::Serialize() {
		String r;
		r.Append(body);
		return r;
	}
	// 誤差避けのためSerializeするときはメモリの形式そのままのダンプ
	template <> String Prefs<float>::Serialize() {
		String r("0x");
		r.Append(*(u32*)&body, '0', 8, 16);
		return r;
	}
	template <> void Prefs<bool>::DeSerialize(const char* t) {
		body = *t == 't' || *t == 'T' || *t == '1' || *t == 'y' || *t == 'Y';
	}
	template <> void Prefs<i8>::DeSerialize(const char* t) {
		body = strtol(t, 0, 10);
	}
	template <> void Prefs<i16>::DeSerialize(const char* t) {
		body = strtol(t, 0, 10);
	}
	template <> void Prefs<i32>::DeSerialize(const char* t) {
		body = strtol(t, 0, 10);
	}
	template <> void Prefs<i64>::DeSerialize(const char* t) {
		body = strtol(t, 0, 10);
	}
	template <> void Prefs<u8>::DeSerialize(const char* t) {
		body = strtoul(t, 0, 10);
	}
	template <> void Prefs<u16>::DeSerialize(const char* t) {
		body = strtoul(t, 0, 10);
	}
	template <> void Prefs<u32>::DeSerialize(const char* t) {
		body = strtoul(t, 0, 10);
	}
	template <> void Prefs<u64>::DeSerialize(const char* t) {
		body = strtoul(t, 0, 10);
	}
	// 0x始まりならダンプ形式
	template <> void Prefs<float>::DeSerialize(const char* t) {
		if (t[0] == '0' && t[1] == 'x') {
			*(u32*)&body = strtoul(t + 2, 0, 16);
		} else {
			body = strtof(t, 0);
		}
	}


	template <> String Prefs<String>::Serialize() { return body; }
	template <> void Prefs<String>::DeSerialize(const char* t) { body = t; }
}
