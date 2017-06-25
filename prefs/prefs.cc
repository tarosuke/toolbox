/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと変数を設定値として管理してくれる便利クラス
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <gdbm.h>

#include <toolbox/prefs.h>
#include <toolbox/complex/complex.h>



namespace{
	GDBM_FILE db;
}


namespace TB{

	CommonPrefs* CommonPrefs::q(0);
	TB::String CommonPrefs::path;

	bool CommonPrefs::Open(){
		//開けなかったら待ってリトライ。何度かやってみてダメなら諦めて戻る
		for(unsigned r(0); r < 5; ++r, sleep(r + 1)){
			if(!!(db = gdbm_open(
				path, 512, GDBM_WRCREAT, 0600, 0))){
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

	CommonPrefs::CommonPrefs(
		const char* key, void* body, unsigned length, Attribute attr) :
		key(key), keyLen(strlen(key) + 1), body(body), length((int)length),
		attr(attr), deleted(false), dirty(false){
		//スタックに自身を追加
		next = q;
		q = this;
	}

	void CommonPrefs::Read(){
		if(attr == nosave){ return; }
		datum k{ const_cast<char*>(key), keyLen };
		datum content(gdbm_fetch(db, k));
		if(content.dptr){
			memcpy(body, content.dptr,  length < content.dsize ? length : content.dsize);
			free(content.dptr);
			dirty = false;
		}
	}
	void CommonPrefs::Write(){
		if(attr == nosave){ return; }
		datum k = { const_cast<char*>(key), keyLen };
		if(!deleted){
			if(dirty){
				datum content = { (char*)body, length };
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
				free(a);
				return true;
			}
		}

		//キーに一致するエントリなし
		free(a);
		return false;
	}



	/** 型ごとの=演算子
	 */
	template<> void Prefs<VECTOR<3> >::operator=(const char* v){
		for(unsigned n(0); n < 3; ++n){
			body[n] = strtod(v, const_cast<char**>(&v));
		}
		Undelete();
	}
	template<> void Prefs<unsigned>::operator=(const char* v){
		body = strtoul(v, 0, 10);
		Undelete();
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
	}


}
