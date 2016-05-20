/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと登録した変数を設定値として管理してくれる便利クラス
 */
#pragma once

#include <string.h>

#include "prefs/db.h"



namespace wO{

	/**
	 * テンプレートでstaticメンバを作っても型ごとにインスタンスができてしまうので
	 * インスタンスを共通化するための親クラス
	 */
	class CommonPrefs{
	protected:
		CommonPrefs();
		~CommonPrefs();
		static DB* db;
		static unsigned refs;
		static const char* fileName; /** ←このインスタンスをどこかに作る */
	};

	/**
	 * 型とキー文字列を与えてインスタンスを作るだけで設定になる変数を作れるクラス
	 * NOTE:keyは文字列リテラルである必要がある
	 * NOTE:取り出しは参照だが代入は「=」でなされる
	 * NOTE:operatorによる変換は用意してはあるが、元の型そのものではない
	 */
	template<typename T, unsigned maxLen=0> class Prefs : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key) : key(key){
			(*db).Fetch(key, &body);
		};
		Prefs(const char* key, const T& defaultValue) : key(key), body(defaultValue){
			(*db).Fetch(key, &body);
		};
		~Prefs(){
			(*db).Store(key, &body);
		};
		operator const T&(){ return body; };
		void operator =(const T& v){ body = v; };

	private:
		const char* key;
		T body;
	};

	template<unsigned maxLen> class Prefs<char*, maxLen> : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key) : key(key){
			(*db).Fetch(key, &body, maxLen);
			body[maxLen - 1] = 0;
		};
		~Prefs(){
			(*db).Store(key, &body);
		};
		operator char*(){ return body; };
		void operator =(const char* v){ strncpy(body, v, maxLen); };

	private:
		const char* key;
		char body[maxLen];
	};




}
