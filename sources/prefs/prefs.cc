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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/file.h>
#include <syslog.h>
#include <toolbox/exception.h>
#include <toolbox/geometry/vector.h>
#include <toolbox/prefs.h>
#include <unistd.h>



namespace {
	struct File {
		File(const char* path, const char* mode) : file(fopen(path, mode)) {
			if (!file) {
				return;
			}

			// ロック取得
			const int fd(fileno(file));
			if (!!flock(fd, LOCK_EX)) {
				// 何かのエラー
				fclose(file);
				file = 0;
				Posit(false);
			}
			// ファイルの変更に備えて先頭に戻しておく
			lseek(fd, 0, SEEK_SET);
		};
		~File() {
			if (file) {
				fclose(file);
			}
		};
		operator FILE*() { return file; };

	private:
		FILE* file;
	};
}


namespace {
	FILE* Open(const char* path, const char* mode) {
		FILE* f(fopen(path, "r"));
		if (f) {
			// ファイルロックを試みる
			if (!!flock(fileno(f), LOCK_EX)) {
				// 何かのエラー
				fclose(f);
				f = 0;
				return 0;
			}
			// ファイルの変更に備えて先頭に戻しておく
			rewind(f);
		}
		return f;
	}
}


namespace TB {

	TB::Path PrefsBase::path;
	PrefsBase* PrefsBase::q(0);

	void PrefsBase::SetValue(char* line) {
		// 行を=でkey/valueに分ける
		char* v(line);
		for (; *v && *v != '='; ++v) {}
		if (*v == '=') {
			*v++ = 0;
		}

		// keyがマッチしたエントリにはLoad
		for (auto* p(q); p; p = p->next) {
			if (!strcmp(line, p->key)) {
				p->Read(v);
			}
		}
	}

	void PrefsBase::Load(int argc, const char* argv[]) {
		// パスのセットアップ
		Path p(argv[0]);
		path = getenv("HOME");
		path += "/.";
		path += p.LastSegment();

		// 設定ファイルを読む
		char line[1024];
		FILE* f(Open(path.c_str(), "r"));
		if (f) {
			while (!!fgets(line, sizeof(line), f)) {
				// key/valueを設定
				SetValue(line);
			}
			fclose(f);
		}

		// コマンドラインを読む
		for (int n(1); n < argc; ++n) {
			const char* const a(argv[n]);
			if (a[0] == '-') {
				if (a[1] == '-') {
					// --key=value形式
					strcpy(line, a);
					SetValue(line);
				} else {
					// -スイッチ形式
					snprintf(line, sizeof(line), "%s=false", a);
					SetValue(line);
				}
			} else if (a[0] == '+') {
				// +スイッチ形式
				snprintf(line, sizeof(line), "%s=true", a);
				SetValue(line);
			} else {
				// それ以外が現れたら終了
				return;
			}
		}
	}

	void PrefsBase::Store() {
		// ファイルを開いてStore
		FILE* f(Open(path.c_str(), "w"));
		if (f) {
			for (PrefsBase* p(q); p; p = p->next) {
				if (~p->attr & nosave) {
					p->Write(f);
				}
			}
			fclose(f);
		}
	}



	PrefsBase::PrefsBase(const char* key, unsigned attr)
		: key(key), attr(attr) {
		// スタックに自身を追加
		next = q;
		q = this;
	}



	/** 型ごとのLoad/Write
	 */
	template <> void Prefs<std::string>::Read(const char* v) { body = v; };
	template <> void Prefs<int>::Read(const char* v) { sscanf(v, "%d", &body); }
	template <> void Prefs<unsigned>::Read(const char* v) {
		sscanf(v, "%u", &body);
	}
	template <> void Prefs<long>::Read(const char* v) {
		sscanf(v, "%ld", &body);
	}
	template <> void Prefs<unsigned long>::Read(const char* v) {
		sscanf(v, "%lu", &body);
	}
	template <> void Prefs<float>::Read(const char* v) {
		sscanf(v, "%f", &body);
	}
	template <> void Prefs<double>::Read(const char* v) {
		sscanf(v, "%lf", &body);
	}
	template <> void Prefs<bool>::Read(const char* v) {
		switch (*v) {
		case 't':
		case 'T':
		case '1':
			body = true;
			break;
		case 'f':
		case 'F':
		case '0':
			body = false;
			break;
		}
	}
	template <> void Prefs<Vector<3, double>>::Read(const char* v) {
		sscanf(v, "%lf %lf %lf", &body[0], &body[1], &body[2]);
	}
	template <> void Prefs<Vector<3, float>>::Read(const char* v) {
		sscanf(v, "%f %f %f", &body[0], &body[1], &body[2]);
	}

	template <> void Prefs<std::string>::Write(FILE* f) {
		fprintf(f, "%s\n", body.c_str());
	}
	template <> void Prefs<int>::Write(FILE* f) {
		fprintf(f, "%s=%d\n", key, body);
	}
	template <> void Prefs<unsigned>::Write(FILE* f) {
		fprintf(f, "%s=%u\n", key, body);
	}
	template <> void Prefs<long>::Write(FILE* f) {
		fprintf(f, "%s=%ld\n", key, body);
	}
	template <> void Prefs<unsigned long>::Write(FILE* f) {
		fprintf(f, "%s=%lu\n", key, body);
	}
	template <> void Prefs<float>::Write(FILE* f) {
		fprintf(f, "%s=%f\n", key, body);
	}
	template <> void Prefs<double>::Write(FILE* f) {
		fprintf(f, "%s=%lf\n", key, body);
	}
	template <> void Prefs<bool>::Write(FILE* f) {
		fprintf(f, "%s=%s\n", key, body ? "true" : "false");
	}
	template <> void Prefs<Vector<3, double>>::Write(FILE* f) {
		fprintf(f, "%s=%lf %lf %lf\n", key, body[0], body[1], body[2]);
	}
	template <> void Prefs<Vector<3, float>>::Write(FILE* f) {
		fprintf(f, "%s=%f %f %f\n", key, body[0], body[1], body[2]);
	}
}
