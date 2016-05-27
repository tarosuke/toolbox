/** 設定管理
 * 設定を与えてstaticなインスタンスを作っておくと登録した変数を設定値として管理してくれる便利クラス
 *
 * 設定はKeeperを作ると読み込まれ、Keeperが消滅すると保存される。
 *
 * 末端の設定に何かさせたい時はPrefsの子クラスを作ってExtendedHandlerをオーバーライドした上で
 * 呼び出す。また全ての設定を辿る時はItorを作って0になるまでループする。
 */
#pragma once

#include <string.h>



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

		//設定を辿るための反復子
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

		// 拡張用ハンドラ
		virtual void ExtendedHandler(void* =0){};

	protected:
		CommonPrefs(const char* key, void* body, unsigned length);
		~CommonPrefs(){};

	private:
		static CommonPrefs* q;
		CommonPrefs* next;

		const char* const key;
		const int keyLen;
		void* const body;
		const int length;

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
	 */
	template<typename T, unsigned maxLen=256> class Prefs : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key) : CommonPrefs(key, (void*)&body, sizeof(T)){};
		Prefs(const char* key, const T& defaultValue) :
			CommonPrefs(key, (void*)&body, sizeof(T)), body(defaultValue){};
		~Prefs(){};

		operator const T&(){ return body; };
		void operator =(const T& v){ body = v; };

	protected:
		const char* key;
		T body;
	};

	template<unsigned maxLen> class Prefs<char*, maxLen> : public CommonPrefs{
		Prefs();
		Prefs(const Prefs&);
		void operator=(const Prefs&);
	public:
		Prefs(const char* key) : CommonPrefs(key, (void*)body, maxLen){
			body[0] = 0;
		};
		Prefs(const char* key, const char* defaultValue) :
			CommonPrefs(key, (void*)body, maxLen){
			*this = defaultValue;
		};
		~Prefs(){};
		operator char*(){ return body; };
		void operator =(const char* v){
			strncpy(body, v, maxLen);
			body[maxLen - 1] = 0;
		};

	protected:
		const char* key;
		char body[maxLen];
	};


}
