/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと登録した変数を設定値として管理してくれる便利クラス
 */

#include <stdlib.h>

#include <string>

#include <wOLIB/debug.h>

#include "../include/toolbox/prefs.h"


namespace wO{

	//データベースと参照カウンタ
	DB* CommonPrefs::db(0);
	unsigned CommonPrefs::refs(0);

	CommonPrefs::CommonPrefs(){
		if(!db){
			//dbが空なので開いておく
			std::string path(getenv("HOME"));
			path += fileName;
DPRINTF("try to open:%s.\n", path.c_str());
			db = new DB(path.c_str());
			refs = 1;
		}
	}

	CommonPrefs::~CommonPrefs(){
		if(!--refs && db){
			delete db;
			db = 0;
		}
	}

}
