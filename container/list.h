/************************************************************************ List
 *
 */

#pragma once

#include "../lock/key.h"



namespace wO{

	template<class T, int index = 0, class L=Lock::NullLock> class List : public L{
	public:
		//ノード：これを継承して使う。複数のListを使うときは多重継承
		class Node{
			friend class List;
			friend class ITOR;
		public:
			~Node(){ Detach(); };
		protected:
			Node(T& org) : prev(this), next(this), origin(&org){};
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
			Lock::Key<L> key;
			Node* node;
		};
		template<typename U> T* Foreach(bool (T::*handler)(U), U param){
			for(ITOR i(*this); ++i && ((*i).*handler)(param););
		};
		void Foreach(void (T::*handler)()){
			for(ITOR i(*this); ++i;){
				((*i).*handler)();
			}
		}
		template<typename U> T* Reveach(bool (T::*handler)(U), U param){
			for(ITOR i(*this); --i && ((*i).*handler)(param););
		};
		void Reveach(void (T::*handler)()){
			for(ITOR i(*this); --i;){
				((*i).*handler)();
			}
		};
		//普通に操作するメソッド
		void Add(Lock::Key<L>&, Node& n){
			n.Insert(anchor);
		};
		void Add(Node& n){
			Lock::Key<L> k(lock);
			Add(k, n);
		};
		void Insert(Lock::Key<L>&, Node& n){
			n.Attach(anchor);
		};
		void Insert(Node& n){
			Lock::Key<L> k(lock);
			Insert(k, n);
		};
		T* Top(Lock::Key<L>&){
			return (*anchor.next).origin;
		};

		T* Get(Lock::Key<L>& k){
			Node* const top(Top(k));
			if(top != &anchor){
				(*top).Detach(k);
			}
			return *top;
		};

	private:
		Node anchor;
		L lock;
	};

}

