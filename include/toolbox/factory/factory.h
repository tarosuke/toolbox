/*** 自動登録ファクトリ
 * 親クラスにstaticな「FACTORY<親クラス>* FACTORY<親クラス>::start(0)」を定義
 * 子クラスにstaticな「FACTORY<親クラス> 子クラス::factory(New)」を定義
 * 子クラスにstaticな「親クラス* New()」を定義
 * 親クラスからFACTORY<親クラス>::Newを呼び出すと子クラスのNewを順に呼んで
 * 値が入ってたらそれを返してくれる
 * Newにはパラメタなしのものと、参照パラメタ一つのものがある。
 *
 * NOTE:一般的にFACTORY<親クラス>::Newを呼ぶのはmainに入った後にすること
 * 登録がグローバルコンストラクタなのでグローバルコンストラクタでNewするには
 * 呼び出し順を調整する必要がある
 */
#pragma once



//基本形
template<class T, typename P=void> class FACTORY{
	FACTORY();
	FACTORY(const FACTORY&);
	void operator=(const FACTORY&);
public:
	FACTORY(T* (*f)(), unsigned (*s)() = 0) :
		next(start),
		factory(f),
		score(s){
		start = this;
	};
	static T* New(P p){
		for(FACTORY* f(start); f; f = (*f).next){
			T* const t(((*f).factory)(p));
			if(t){
				return t;
			}
		}
		return 0;
	}
private:
	static FACTORY* start;
	FACTORY* const next;
	T* (*const factory)(P);
	unsigned (*const score)(P);
};

//引数なしの場合
template<class T> class FACTORY<T, void>{
	FACTORY();
	FACTORY(const FACTORY&);
	void operator=(const FACTORY&);
public:
	FACTORY(T* (*f)(), unsigned (*s)() = 0) :
		next(start),
		factory(f),
		score(s){
			start = this;
		};
	static T* New(){
		for(FACTORY* f(start); f; f = (*f).next){
			T* const t(((*f).factory)());
			if(t){
				return t;
			}
		}
		return 0;
	};
private:
	static FACTORY* start;
	FACTORY* const next;
	T* (*const factory)();
	unsigned (*const score)();
};
