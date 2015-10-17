/************************************************************************ List
 *
 */

#pragma once

#include "../lock/key.h"



namespace wO{

	template<class T, int index = 0, class L=Lock::NullLock> class List : public L{
	public:
		//このリストのためのKey(このリストを初期値にして作る)
		typedef Lock::Key<L> Key;
		//ノード：これを継承して使う。複数のListを使うときは多重継承
		//List自体がdeleteするとNotifyListDeletedが呼ばれる
		class Node{
			friend class List;
			friend class ITOR;
			Node(const Node&);
			void operator=(const Node&);
		public:
			virtual ~Node(){ Detach(); };
		protected:
			Node(T& org) : prev(this), next(this), origin(&org){};
			virtual void NotifyListDeleted(){};
		private:
			Node* prev;
			Node* next;
			T* const origin;
			Node() : prev(this), next(this), origin(0){}; //anchor用
			void Insert(Node& n){
				if(prev != this){
					Detach();
				}
				prev = n.prev;
				next = &n;
				(*prev).next = n.prev = this;
			};
			void Attach(Node& n){
				if(prev != this){
					Detach();
				}
				next = n.next;
				prev = &n;
				(*next).prev = n.next = this;
			};
			void Detach(){
				(*prev).next = next;
				(*next).prev = prev;
				prev = next = this;
			};
			operator T*(){
				return origin;
			};
		};
		//STLよりJavaのそれに近い反復子
		class ITOR{
		public:
			ITOR(List& l) : key(l), node(&l.anchor){};
			operator T*(){
				return *node;
			};
			T* operator++(){
				node = (*node).next;
				return *node;
			};
			T* operator--(){
				node = (*node).prev;
				return *node;
			};
		private:
			Key key;
			Node* node;
		};
		template<typename U> T* Foreach(bool (T::*handler)(U&), U& param){
			for(ITOR i(*this); ++i;){
				if(((*i).*handler)(param)){
					return i;
				}
			}
			return 0;
		};
		template<typename U> void Foreach(void (T::*handler)(U&), U& param){
			for(ITOR i(*this); ++i;){
				((*i).*handler)(param);
			}
		};
		void Foreach(void (T::*handler)()){
			for(ITOR i(*this); ++i;){
				((*i).*handler)();
			}
		}
		template<typename U> T* Reveach(bool (T::*handler)(U&), U& param){
			for(ITOR i(*this); --i;){
				if(((*i).*handler)(param)){
					return i;
				}
			}
			return 0;
		};
		template<typename U> void Reveach(void (T::*handler)(U&), U& param){
			for(ITOR i(*this); --i;){
				((*i).*handler)(param);
			}
		};
		void Reveach(void (T::*handler)()){
			for(ITOR i(*this); --i;){
				((*i).*handler)();
			}
		};

		//普通に操作するメソッド
		void Add(Key&, Node& n){
			n.Insert(anchor);
		};
		void Add(Node& n){
			Key k(*this);
			Add(k, n);
		};
		void Insert(Key&, Node& n){
			n.Attach(anchor);
		};
		void Insert(Node& n){
			Key k(*this);
			Insert(k, n);
		};
		T* Top(Key&){
			return (*anchor.next).origin;
		};
		T* Bottom(Key&){
			return (*anchor.prev).origin;
		};

		T* Get(Key&){
			Node* const tn(anchor.next);
			if(tn != &anchor){
				(*tn).Detach();
				return (*tn).origin;
			}
			return 0;
		};
		T* Get(){
			Key key(*this);
			return Get(key);
		};

		//コンストラクタ／デストラクタ
		~List(){
			while(anchor.next != &anchor){
				Node* const n(anchor.next);
				if(n == &anchor){
					continue;
				}
				(*n).Detach();
				(*n).NotifyListDeleted();
			}
		};

	private:
		Node anchor;
	};

}

