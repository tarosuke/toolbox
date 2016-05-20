/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと登録した変数を設定値として管理してくれる便利クラス
 */

#include <stdlib.h>
#include <string>
#include <gdbm/gdbm.h>

#include <wOLIB/debug.h>

#include "../include/toolbox/prefs.h"



namespace{
	GDBM_FILE db;
}


namespace wO{

	CommonPrefs* CommonPrefs::q(0);

	void CommonPrefs::Load(const char* name){
		//データベースのパス生成
		std::string path(getenv("HOME"));
		path += "/";
		path += name;

		//データベースを開く
		if(!(db = gdbm_open(path.c_str(),  512, GDBM_WRCREAT, 0600, 0))){
			//開けなかった
			DPRINTF("Pref database %s could not open.\n", path.c_str());
			return;
		}

		for(CommonPrefs* i(q); i; i = (*i).next){
			(*i).Read();
		}
	}

	void CommonPrefs::Store(){
		if(!db){ return; }

		for(CommonPrefs* i(q); i; i = (*i).next){
			(*i).Write();
		}
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
		}else{
			DPRINTF("Pref key %s could not read.\n", key);
		}
	}
	void CommonPrefs::Write(){
		datum k = { const_cast<char*>(key), keyLen };
		datum content = { (char*)body, length };
		gdbm_store(db, k, content, GDBM_REPLACE);
	}


}
