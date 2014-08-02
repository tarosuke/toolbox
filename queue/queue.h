/************************************************ bi-directional linked list:Q
 *
 *
 */

#ifndef _QUEUE_
#define _QUEUE_

#include "../key/key.h"

namespace TOOLBOX{

template<class T, class LOCK = NULLLOCK> class NODE{ //TODO:ロック対応改修
public:
	NODE(T& owner) : owner(&owner), next(this), prev(this){}; //TODO:Tの通知メソッドを設定。デフォルトは0
	~NODE(){ Detach(); };
	//thisをnの前に接続
	void Insert(NODE& n){
		if(prev != this){
			Detach();
		}
		prev = n.prev;
		next = &n;
		n.prev = (*prev).next = this;
	};
	//thisをnの後に接続
	void Attach(NODE& n){
		if(prev != this){
			Detach();
		}
		next = n.next;
		prev = &n;
		n.next = (*next).prev = this;
	};
	//thisをどこかから外す(どこにも繋がっていなくてもおｋ)
	void Detach(){
		(*next).prev = prev;
		(*prev).next = next;
		prev = next = this;
	};
	operator T*(){ return owner; };
	inline T* Owner() const{ return owner; };
	NODE* Next(){ return next; };
	NODE* Prev(){ return prev; };
	void Notify(){ /*TODO:初期化時に設定された通知メソッドのポインタを呼ぶ*/ };
protected:
	NODE() : owner(0), next(this), prev(this){}; //QUEUEのため
	T* const owner;
	NODE* next;
	NODE* prev;
};


template<class T, class LOCK = NULLLOCK> class QUEUE : public NODE<T>{
public:
	class ITOR{
		/** インスタンスが存在する間は対象キューが変化しないことが保証される
		 * が、その間他の一切の操作はブロックされるのでキュー操作はすべて
		 * ITORを通す必要がある。
		 * それと、ITORが指しているノードはDetachしないこと。
		 * Detachする必要があるならノードを保存したあとITORを一つ進めてから。
		 */
	public:
		ITOR(QUEUE& q) : q(&q), n(q.next), key(q.lock){};
		T* operator++(int){
			if(n != q){
				T* t((*n).Owner());
				n = (*n).Next();
				return t;
			}
			return 0;
		};
		operator T*(){
			return *n;
		};
		inline T* Owner(){
			return (*n).Owner();
		}
		void Insert(NODE<T>& node){
			//ITORが指すノードの前にnodeを追加する
			if(!n){
				(*q).Insert(node);
			}else{
				(*n).Insert(node);
			}
		};
		void Add(NODE<T>& node){
			//ITORが指すノードの後にnodeを追加する
			if(!n){
				(*q).Add(node);
			}else{
				(*n).Attach(node);
			}
		};
		T* Detach(){
			//ITORが指すノードを除去し、次の要素を指す
			if(n != q){
				T* const t((*n).Owner());
				n = (*n).Next();
				return t;
			}
			//ITORが何も指していなければ0を返す
			return 0;
		};
		operator bool(){
			return !!(*n).Owner();
		};
	private:
		NODE<T>* const q;
		NODE<T>* n;
		KEY<LOCK> key;
	};
	class RITOR{
		/** インスタンスが存在する間は対象キューが変化しないことが保証される
		  * が、その間他の一切の操作はブロックされるのでキュー操作はすべて
		  * ITORを通す必要がある
		  */
	public:
		RITOR(QUEUE& q) : q(&q), n(q.prev), key(q.lock){};
		T* operator--(int){
			if(n != q){
				T* t((*n).Owner());
				n = (*n).Prev();
				return t;
			}
			return 0;
		};
		operator T*(){
			return *n;
		};
		inline T* Owner(){
			return (*n).Owner();
		}
		operator bool(){
			return !!(*n).Owner();
		};
	private:
		NODE<T>* const q;
		NODE<T>* n;
		KEY<LOCK> key;
	};
	template<typename U> T* Scan(bool (T::*isThis)(const U), const U key) const{
		for(ITOR i(*this); i; i++){
			if((*i).*isThis(key)){
				return i;
			}
		}
		return 0;
	};
	bool IsThere(){ KEY<LOCK> key(lock); return IsThere(key); };
	T* Get(){ KEY<LOCK> key(lock); return Get(key); };
	void Add(NODE<T>& n){ KEY<LOCK> key(lock); Add(key, n); };
	void Insert(NODE<T>& n){ KEY<LOCK> key(lock); Insert(key, n); };
	T* Peek() const{ return (*NODE<T>::next).Owner(); };
	void Pick(NODE<T>& n){
		KEY<LOCK> key(lock);
		if(&n != NODE<T>::next){
			n.Detach();
			Insert(key, n);
		}
	};
	~QUEUE(){
		//TODO:各ノードにおいてリンクを外しつつnodeに設定されている通知メソッドを呼ぶ
// 		while(next != this){
// 			NODE<T>* n(next);
// 			(*next).Detach();
// 			(*n).Notify();
// 		}
	};
	template<typename J> void Each(J job){
		for(ITOR i(*this); i;){
			T& t(*i);
			i++;
			(t.*job)();
		}
	};
	template<typename J, typename K> void Each(J job, K& param){
		for(ITOR i(*this); i;){
			T& t(*i);
			i++;
			(t.*job)(param);
		}
	};
private:
	LOCK lock;
	inline bool IsThere(KEY<LOCK>&){
		return (*this).next != this;
	};
	T* Get(KEY<LOCK>& key){
		if(IsThere(key)){
			T* o((*this).owner);
			(*(*this).next).Detach();
			return o;
		}
		return 0;
	};
	inline void Add(KEY<LOCK>&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの前は最後
		n.Insert(*this);
	};
	inline void Insert(KEY<LOCK>&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの後は最初
		n.Attach(*this);
	};
};


template<typename T, unsigned max, class LOCK = NULLLOCK> class MULTIQUEUE{
public:
	T* Get(unsigned e = max){
		KEY<LOCK> key(lock);
		for(unsigned i(0); i < e; i++){
			if(q[i].IsThere()){
				return q[i].Get();
			}
		}
		return 0;
	};
	void Add(unsigned index, NODE<T>& node){
		KEY<LOCK> key(lock);
		q[index].Add(node);
	};
	void Insert(unsigned index, NODE<T>& node){
		KEY<LOCK> key(lock);
		q[index].Insert(node);
	};
	unsigned GetMax(){
		KEY<LOCK> key(lock);
		for(unsigned i(0); i < max; i++){
			if(q[i].IsThere()){
				return i;
			}
		}
		return max;
	};
private:
	QUEUE<T> q[max];
	LOCK lock;
};

};
#endif
