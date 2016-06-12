/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと登録した変数を設定値として管理してくれる便利クラス
 */

#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <gdbm/gdbm.h>

#include "../include/toolbox/prefs.h"



namespace{
	GDBM_FILE db;
	std::string path;
}


namespace TB{

	CommonPrefs* CommonPrefs::q(0);

	bool CommonPrefs::Open(){
		//開けなかったら待ってリトライ。何度かやってみてダメなら諦めて戻る
		for(unsigned r(0); r < 5; ++r, sleep(r + 1)){
			if(!!(db = gdbm_open(path.c_str(),  512, GDBM_WRCREAT, 0600, 0))){
				//開けたので戻る
				return true;
			}
		}
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

	CommonPrefs::CommonPrefs(const char* key, void* body, unsigned length) :
		key(key), keyLen(strlen(key) + 1), body(body), length((int)length){
		//スタックに自身を追加
		next = q;
		q = this;
	}

	void CommonPrefs::Read(){
		datum k{ const_cast<char*>(key), keyLen };
		datum content(gdbm_fetch(db, k));
		if(content.dptr){
			memcpy(body, content.dptr,  length < content.dsize ? length : content.dsize);
			free(content.dptr);
		}
	}
	void CommonPrefs::Write(){
		datum k = { const_cast<char*>(key), keyLen };
		datum content = { (char*)body, length };
		gdbm_store(db, k, content, GDBM_REPLACE);
	}


}
