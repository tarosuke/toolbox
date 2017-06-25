/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと変数を設定値として管理してくれる便利クラス
 *
 * 設定はKeeperを作ると読み込まれ、Keeperが消滅すると保存される。
 *
 * 末端の設定に何かさせたい時はPrefsの子クラスを作ってExtendedHandlerをオーバーライドした上で
 * 呼び出す。また全ての設定を辿る時はItorを作って0になるまでループする。
 */
#pragma once

#include <string.h>
#include <toolbox/string.h>



namespace TB{

	/**
	 * テンプレートでstaticメンバを作っても型ごとにインスタンスができてしまうので
	 * インスタンスを共通化するための親クラス
	 */
	class CommonPrefs{
		CommonPrefs();
		CommonPrefs(const CommonPrefs&);
		void operator=(const CommonPrefs&);
	public:
		//属性値
		enum Attribute{
			save,
			nosave,
		};

		//設定の読み込みと保存キー
		class Keeper{
			Keeper();
			Keeper(const Keeper&);
			void operator=(const Keeper&);
		public:
			Keeper(const char* name){
				Load(name);
			};
			~Keeper(){
				Store();
			};
		};

		/**設定を辿るための反復子
		 * NOTE:初期化時点で先頭を指しているのでC++式反復子形式(処理後インクリメント)で使う
		 */
		class Itor{
			Itor(const Itor&);
			void operator=(const Itor&);
		public:
			Itor() : node(q){};
			operator CommonPrefs*(){ return node; };
			void operator++(){ if(node){ node = (*node).next; } };
		private:
			CommonPrefs* node;
		};

		//拡張用ハンドラ
		virtual void ExtendedHandler(void* =0){};

		//文字列による値設定(要するにコマンドラインオプション用)
		static bool Set(const char* arg);

	protected:
		CommonPrefs(
			const char* key,
			void* body,
			unsigned length,
			Attribute attr);
		~CommonPrefs(){};

		virtual void operator=(const char*)=0;
		void Delete(){ deleted = true; }
		void Undelete(){ deleted = false; }
		bool IsDeleted(){ return deleted; }
		void Waste(){ dirty = true; };

	private:
		static TB::String path;

		static CommonPrefs* q;
		CommonPrefs* next;

		const char* const key;
		const int keyLen;
		void* const body;
		const int length;
		const Attribute attr;
		bool deleted;
		bool dirty;

		static bool Open();
		static void Load(const char*);
		static void Store();

		void Read();
		void Write();
	};

	/**
	 * 型とキー文字列を与えてインスタンスを作るだけで設定になる変数を作れるクラス
	 * NOTE:keyは文字列リテラルである必要がある
	 * NOTE:「=」で代入できる必要がある
	 * NOTE:operatorによる変換は用意してはあるが、代入以外はキャストする必要がある
	 * NOTE:処理がmainに入るまでは機能しない
	 */
	template<typename T, unsigned maxLen=256> class Prefs : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key, Attribute attr=save) :
			CommonPrefs(key, (void*)&body, sizeof(T), attr){};
		Prefs(const char* key, const T& defaultValue, Attribute attr=save) :
			CommonPrefs(key, (void*)&body, sizeof(T), attr),
			body(defaultValue), defaultValue(defaultValue){};
		~Prefs(){};

		operator const T&(){ return IsDeleted() ? defaultValue : body; };
		void operator=(const T& v){ body = v; Undelete(); Waste(); };
		void operator=(const char* v) override;

	protected:
		const char* key;
		T body;
		T defaultValue;
	};

	template<unsigned maxLen> class Prefs<char*, maxLen> : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key, Attribute attr=save) :
			CommonPrefs(key, (void*)body, maxLen, attr),
			defaultValue(0){
			body[0] = 0;
		};
		Prefs(const char* key, const char* defaultValue, Attribute attr=save) :
			CommonPrefs(key, (void*)body, maxLen, attr),
			defaultValue(defaultValue){
			*this = defaultValue;
		};
		~Prefs(){};
		operator const char*(){ return IsDeleted() ? defaultValue : body; };
		void operator=(const char* v) override{
			strncpy(body, v, maxLen);
			body[maxLen - 1] = 0;
			Undelete();
			Waste();
		};

	protected:
		const char* key;
		char body[maxLen];
		const char* const defaultValue;
	};


}
