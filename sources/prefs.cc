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
			if (!!strncmp(i->key, line, len)) {
				// 形式チェック
				if (len < 1 || line[len - 1] != '=') {
					syslog(LOG_ALERT, "invalid option:%s", line);
					return;
				}
				i->DeSerialize(line + len);
			}
		}
	}

	void CommonPrefs::Load(int argc, const char** argv, const char* n) {
		// nが指定されていたらそれを、でなければargv[0]のファイル名を使う
		String name(n ? n : Path::Base(argv[0]));

		// 設定ファイルのパスを生成
		const char* const home(getenv("HOME"));
		path = home ? home : "/root";
		path += "/";
		path += name;

		FILE* const file(fopen(path.c_str(), "r"));

		if (!file) {
			return;
		}

		for (char b[256]; fgets(b, sizeof(b), file);) {
			LoadLine(b);
		}
		fclose(file);
	}

	void CommonPrefs::Parse(int argc, const char** argv) {
		for (int n(1); n < argc; ++n) {
			const char* const arg(argv[n]);
			String line;

			if (arg[0] != '-' && arg[0] != '+') {
				// 解釈できる形式は終了
				return;
			}

			// キーの取り出し
			String key(arg);
			String value;
			if (arg[0] == '-' && arg[1] == '=') {
				const auto len(strlen(arg));
				if (arg[len - 1] == '=') {
					// 値オプション(--xxx=yyy)
					line = arg;
				} else {
					// 値オプション(--xxx yyy)
					if (argc <= ++n) {
						syslog(LOG_CRIT, "option %s needs parametor", arg);
						return;
					}
					line = arg;
					line += '=';
					line = argv[n];
				}
			} else {
				// スイッチオプション(-/+xxx)
				line = '-';
				line += &arg[1];
				line += '=';
				line += arg[0] == '-' ? 'f' : 't';
			}

			LoadLine(line.c_str());
		}
	}

	void CommonPrefs::Store() {
		FILE* const file(fopen(path.c_str(), "r"));

		if (!file) {
			return;
		}

		for (CommonPrefs* i(q); i; i = (*i).next) {
			if (i->attr == save) {
				String line(i->key);
				line += '=';
				line += i->Serialize();
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

}
