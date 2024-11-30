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
 */

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <tb/prefs.h>
#include <tb/string.h>

namespace tb {

	CommonPrefs* CommonPrefs::q(0);
	std::filesystem::path
		CommonPrefs::pathes[(unsigned)CommonPrefs::Level::nLevel];

	void CommonPrefs::LoadLine(unsigned level, const std::string& line) {
		const unsigned delim(line.find_first_of('='));
		const std::string key(line.substr(0, delim));
		const std::string value(line.substr(delim + 1));
		for (CommonPrefs* i(q); i; i = (*i).next) {
			if (!strcmp(key.c_str(), i->key)) {
				i->DeSerialize(level, value.c_str());
			}
		}
	}

	void CommonPrefs::Load(int argc, const char** argv, const char* n) {
		// nが指定されていたらそれを、でなければargv[0]のファイル名を使う
		const std::filesystem::path base(
			std::filesystem::path(argv[0]).filename());

		// カレントを設定
		pathes[(unsigned)Level::current] =
			std::filesystem::current_path() / base;

		// 設定ファイルのパスを生成
		pathes[(unsigned)Level::home] =
			std::filesystem::path(getenv("HOME")) / ".config/" / base;

		// システムファイルのパスを生成
		pathes[(unsigned)Level::etc] = std::filesystem::path("/etc/") / base;

		// コマンドラインと設定読み込み
		Parse(argc, argv);
		for (unsigned n(1); n < (unsigned)Level::nLevel; ++n) {
			Load(n);
		}
	}

	void CommonPrefs::Load(unsigned level) {
		const std::filesystem::path& p(pathes[level]);
		if (!*p.c_str()) {
			// 空：たぶんLevel::value
			return;
		}

		std::ifstream file(p, std::ios_base::in);
		if (!file) {
			// 開けなかった：たぶんファイルがない
			return;
		}

		// 設定ファイルを行ごとに読む
		for (std::string line; getline(file, line);) {
			LoadLine(level, line);
		}
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

			LoadLine((unsigned)Level::value, line.c_str());
		}
		return argc;
	}

	void CommonPrefs::Store() {
		for (unsigned n(1); n < (unsigned)Level::nLevel; ++n) {
			// dirtyチェック
			bool dirty(false);
			for (CommonPrefs* i(q); i; i = i->next) {
				if (i->IsDirty(n)) {
					dirty = true;
					break;
				}
			}
			if (!dirty) {
				continue;
			}

			// ファイルを開いて
			FILE* const file(fopen(pathes[n].c_str(), "w"));

			if (!file) {
				break;
			}

			// dirtyな項目だけ書き込む
			for (CommonPrefs* i(q); i; i = (*i).next) {
				if (i->IsDirty(n) && i->attr == save) {
					String line(i->key);
					line += '=';
					line += i->Serialize(Level::value);
					line += '\n';
					fputs(line.c_str(), file);
				}
			}
			fclose(file);
		}
	}

	CommonPrefs::CommonPrefs(
		const char* key, const char* description, Attribute attr)
		: key(key),
		  attr(attr) {
		// スタックに自身を追加
		next = q;
		q = this;
	}
}
