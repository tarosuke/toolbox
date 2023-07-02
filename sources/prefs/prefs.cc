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
				Assume(false);
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

	TB::Path PrefsBase::paths[];
	PrefsBase* PrefsBase::q(0);

	void PrefsBase::SetValue(unsigned n, char* line) {
		// 行を=でkey/valueに分ける
		char* v(line);
		for (; *v && *v != '='; ++v) {}
		if (*v == '=') {
			*v++ = 0;
		}

		// keyがマッチしたエントリにはLoad
		for (auto* p(q); p; p = p->next) {
			if (!strcmp(line, p->key)) {
				p->Read(n, v);
			}
		}
	}

	void PrefsBase::Load(int argc, const char* argv[]) {
		// パスのセットアップ
		Path p(argv[0]);
		paths[0] = "/etc/";
		paths[1] = getenv("HOME");
		paths[1] += "/.";
		std::string pname = p.LastSegment();

		// 設定ファイルを読む
		char line[1024];
		for (unsigned n(0); n < 3; ++n) {
			if (!paths[n].length()) {
				continue;
			}
			paths[n] += pname;
			FILE* f(Open(paths[n].c_str(), "r"));
			if (f) {
				while (!!fgets(line, sizeof(line), f)) {
					// key/valueを設定
					SetValue(n, line);
				}
				fclose(f);
			}
		}

		// コマンドラインを読む
		for (int n(1); n < argc; ++n) {
			const char* const a(argv[n]);
			if (a[0] == '-') {
				if (a[1] == '-') {
					// --key=value形式
					strcpy(line, a);
					SetValue(3, line);
				} else {
					// -スイッチ形式
					snprintf(line, sizeof(line), "%s=false", a);
					SetValue(3, line);
				}
			} else if (a[0] == '+') {
				// +スイッチ形式
				snprintf(line, sizeof(line), "%s=true", a);
				SetValue(3, line);
			} else {
				// それ以外が現れたら終了
				return;
			}
		}
	}

	void PrefsBase::Store() {
		// userとworkplaceについてファイルを開いてStore
		for (unsigned n(1); n < 3; ++n) {
			FILE* f(Open(paths[n].c_str(), "w"));
			if (f) {
				for (PrefsBase* p(q); p; p = p->next) {
					p->Write(n, f);
				}
				fclose(f);
			}
		}
	}



	PrefsBase::PrefsBase(const char* key) : key(key) {
		// スタックに自身を追加
		next = q;
		q = this;
	}



	/** 型ごとのLoad/Write
	 */
	template <> void Prefs<std::string>::Body::Read(const char* v) {
		body = v;
		valid = true;
	};
	template <> void Prefs<int>::Body::Read(const char* v) {
		sscanf(v, "%d", &body);
		valid = true;
	}
	template <> void Prefs<unsigned>::Body::Read(const char* v) {
		sscanf(v, "%u", &body);
		valid = true;
	}
	template <> void Prefs<long>::Body::Read(const char* v) {
		sscanf(v, "%ld", &body);
		valid = true;
	}
	template <> void Prefs<unsigned long>::Body::Read(const char* v) {
		sscanf(v, "%lu", &body);
		valid = true;
	}
	template <> void Prefs<float>::Body::Read(const char* v) {
		sscanf(v, "%f", &body);
		valid = true;
	}
	template <> void Prefs<double>::Body::Read(const char* v) {
		sscanf(v, "%lf", &body);
		valid = true;
	}
	template <> void Prefs<bool>::Body::Read(const char* v) {
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
		valid = true;
	}
	template <> void Prefs<Vector<3, double>>::Body::Read(const char* v) {
		sscanf(v, "%lf %lf %lf", &body[0], &body[1], &body[2]);
		valid = true;
	}
	template <> void Prefs<Vector<3, float>>::Body::Read(const char* v) {
		sscanf(v, "%f %f %f", &body[0], &body[1], &body[2]);
		valid = true;
	}

	template <> void Prefs<std::string>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s\n", body.c_str());
		dirty = false;
	}
	template <> void Prefs<int>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%d\n", key, body);
		dirty = false;
	}
	template <> void Prefs<unsigned>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%u\n", key, body);
		dirty = false;
	}
	template <> void Prefs<long>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%ld\n", key, body);
		dirty = false;
	}
	template <>
	void Prefs<unsigned long>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%lu\n", key, body);
		dirty = false;
	}
	template <> void Prefs<float>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%f\n", key, body);
		dirty = false;
	}
	template <> void Prefs<double>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%lf\n", key, body);
		dirty = false;
	}
	template <> void Prefs<bool>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%s\n", key, body ? "true" : "false");
		dirty = false;
	}
	template <>
	void Prefs<Vector<3, double>>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%lf %lf %lf\n", key, body[0], body[1], body[2]);
		dirty = false;
	}
	template <>
	void Prefs<Vector<3, float>>::Body::Write(const char* key, FILE* f) {
		if (!dirty) {
			return;
		}
		fprintf(f, "%s=%f %f %f\n", key, body[0], body[1], body[2]);
		dirty = false;
	}
}
