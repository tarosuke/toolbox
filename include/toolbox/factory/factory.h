/*** 自動登録ファクトリ
 * 親クラスに「template<>「FACTORY<親クラス>* FACTORY<親クラス>::start(0)」と、
 * 子クラスにstaticな「FACTORY<親クラス> 子クラス::factory(New)」を定義
 * 子クラスにstaticな「親クラス* New()」を定義
 * 親クラスからFACTORY<親クラス>::Newを呼び出すと子クラスのNewを順に呼んで
 * 値が入ってたらそれを返してくれる
 * Newにはパラメタなしのものと、参照パラメタ一つのものがある。
 *
 * また、scoreに スコア用メソッドを設定すると全てをスキャンして最もスコアが高かった
 * もののNewが呼ばれる。同一スコアの場合は先着順(どちらが先かは規定しない)
 * なお、scoreを設定されていないものがある場合の挙動はscoreなしのものとなるので
 * スコアによる選択を使うときは全てにscoreを設定すること。
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
	FACTORY(T* (*f)(P&), unsigned (*s)(P&) = 0) :
		next(start),
		factory(f),
		score(s){
		start = this;
	};
	static T* New(P& p){
		FACTORY* matched(0);
		unsigned maxScore(0);
		for(FACTORY* f(start); f; f = (*f).next){
			if((*f).score){
				const unsigned s((*f).score(p));
				if(maxScore < s){
					matched = f;
					maxScore = s;
				}
			}else{
				T* const t(((*f).factory)(p));
				if(t){
					return t;
				}
			}
		}
		return (matched && maxScore) ? (*matched).factory(p) : 0;
	}
private:
	static FACTORY* start;
	FACTORY* const next;
	T* (*const factory)(P&);
	unsigned (*const score)(P&);
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
		FACTORY* matched(0);
		unsigned maxScore(0);
		for(FACTORY* f(start); f; f = (*f).next){
			if((*f).score){
				const unsigned s((*f).score());
				if(maxScore < s){
					matched = f;
					maxScore = s;
				}
			}else{
				T* const t(((*f).factory)());
				if(t){
					return t;
				}
			}
		}
		return (matched && maxScore) ? (*matched).factory() : 0;
	};
private:
	static FACTORY* start;
	FACTORY* const next;
	T* (*const factory)();
	unsigned (*const score)();
};
