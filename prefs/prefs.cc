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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <gdbm.h>
#include <syslog.h>

#include <toolbox/prefs.h>
#include <toolbox/complex/complex.h>



namespace{
	GDBM_FILE db;
}


namespace TB{

	CommonPrefs* CommonPrefs::q(0);
	bool CommonPrefs::inited(false);
	TB::String CommonPrefs::path;

	bool CommonPrefs::Open(){
		//排他で開く。開けなかったら待ってリトライ。何度かやってみてダメなら諦めて戻る
		for(unsigned r(0); r < 10; ++r, sleep(r/2 + 1)){
			if(!!(db = gdbm_open(path, 512, GDBM_WRCREAT, 0600, 0))){
				//開けたので戻る
				return true;
			}
		}
		syslog(LOG_ERR, "Pref:failed to pen:%s", (const char*)path);
		return false;
	}

	void CommonPrefs::Load(const char* name){
		//名前チェック(指定されていなければ処理なし)
		if(!name || !*name){
			q = 0; //Storeも処理なしにするためスタックを壊しておく
			return;
		}

		//設定の有無をチェック(設定がなければ処理なし)
		if(!q){ return; }

		//データベースのパス生成
		const char* const home(getenv("HOME"));
		path = home ? home : "/root";
		path += "/";
		path += name;

		//データベースを開く
		if(!Open()){
			//開けなかった
			return;
		}

		for(CommonPrefs* i(q); i; i = (*i).next){
			(*i).Read();
		}

		//閉じておく
		gdbm_close(db);
		db = 0;
	}

	int CommonPrefs::Parse(int argc, const char** argv){
		if(!argc || !argv){
			return 0;
		}

		for(int n(1); n < argc; ++n){
			const char* const arg(argv[n]);

			//コマンドラインオプションの解釈
			if(!TB::CommonPrefs::Set(arg)){
				syslog(LOG_CRIT, "Unknown option: %s", arg);
				return -1;
			}
		}

		return argc;
	}

	void CommonPrefs::Store(){
		//設定の有無をチェック(設定がなければ処理なし)
		if(!q){ return; }

		//データベースを開く
		if(!Open()){
			//開けなかった
			return;
		}

		for(CommonPrefs* i(q); i; i = (*i).next){
			(*i).Write();
		}

		gdbm_close(db);
		db = 0;
	}

	CommonPrefs::CommonPrefs(const char* key, Attribute attr) :
		key(key), keyLen(strlen(key) + 1),attr(attr), deleted(false), dirty(false){
		//読み込み前ならスタックに自身を追加
		if(!inited){
			next = q;
			q = this;
		}
	}

	void CommonPrefs::Read(){
		datum k{ const_cast<char*>(key), keyLen };
		datum content(gdbm_fetch(db, k));
		if(content.dptr){
			AfterRead(content.dptr, content.dsize);
			free(content.dptr);
			dirty = false;
		}
	}
	void CommonPrefs::WriteRecord(const void* d, unsigned l){
		if(attr == nosave){ return; }
		datum k = { const_cast<char*>(key), keyLen };
		if(!deleted){
			if(dirty){
				datum content = { (char*)d, (int)l };
				gdbm_store(db, k, content, GDBM_REPLACE);
			}
		}else{
			gdbm_delete(db, k);
		}
	}

	bool CommonPrefs::Set(const char* arg){
		if(!arg || !*arg){ return false; }

		//トークンを切るために複製
		char* a(strdup(arg));
		if(!a){ return false; }

		//キーと中身に分割
		const char* key(strtok(a, "="));
		const char* val(strtok(NULL, ""));

		//キー存在チェック
		if(!key){ free(a); return false; };

		//スイッチタイプの場合key、valを修正する
		switch(a[0]){
		case '-' :
			if(a[1] != '-'){
				val = "f";
				a[0] = '+';
			}
			break;
		case '+' :
			val = "t";
			break;
		}

		//キー探索
		for(Itor i; i; ++i){
			if(!strcmp((*i).key, key)){
				//一致
				if(val){
					//値を設定
					*i = val;
				}else{
					(*i).Delete();;
				}

				//正常終了
				(*i).Waste();
				free(a);
				return true;
			}
		}

		//キーに一致するエントリなし
		free(a);
		return false;
	}

	/** 引数全部を設定
	 */
	bool CommonPrefs::SetAll(int argc, const char* argv[]){
		for(int n(0); n < argc; ++n){
			if(!Set(argv[n])){
				return false;
			}
		}
		return true;
	}



	/** 型ごとの=演算子
	 */
	template<> void Prefs<VECTOR<3> >::operator=(const char* v){ //TODO:VECTORへ移動
		for(unsigned n(0); n < 3; ++n){
			body[n] = strtod(v, const_cast<char**>(&v));
		}
		Undelete();
		Waste();
	}
	template<> void Prefs<unsigned>::operator=(const char* v){
		body = strtoul(v, 0, 10);
		Undelete();
		Waste();
	}
	template<> void Prefs<float>::operator=(const char* v){
		body = atof(v);
		Undelete();
		Waste();
	}
	template<> void Prefs<bool>::operator=(const char* v){
		switch(*v){
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
		Undelete();
		Waste();
	}


}
