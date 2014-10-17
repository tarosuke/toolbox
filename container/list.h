/************************************************************************ List
 *
 */

#pragma once

#include "../lock/key.h"


namespace Container{

	template<class T, class L=Lock::NullLock> class List : public L{
	public:
		class Node{
			friend class ITOR;
			friend class List;
		public:
			Node(T& o) : prev(this), next(this), origin(&o), lock(0){};
			~Node(){ Detach(); };
			void Add(Node& node, L& l){ //nodeのあとに追加
				Detach();
				lock = &l;
				Lock::Key<L> k(*l);
				prev = &node;
				next = node.next;
				node.next = this;
			};
			void Insert(Node& node, L& l){ //nodeの前に挿入
				Detach();
				lock = &l;
				Lock::Key<L> k(*l);
				next = &node;
				prev = node.prev;
				node.prev = this;
			};
			void Detach(){
				if(lock){
					Lock::Key<L> k(*lock);
					(*next).prev = prev;
					(*prev).next = next;
					prev = next = this;
					lock = 0;
				}
			};
		private:
			Node() : prev(this), next(this), origin(0), lock(0){};
			Node* prev;
			Node* next;
			T* const origin;
			L* lock;
			void Detach(Lock::Key<L>&){
				if(lock){
					(*next).prev = prev;
					(*prev).next = next;
					prev = next = this;
					lock = 0;
				}
			};
		};
		class ITOR{
		public:
			ITOR(List& l) : key(l), node(l.anchor), anchor(&l.anchor){};
			operator T*(){
				return (*node).origin;
			};
			T* operator++(){
				node = (*node).next;
				return (*node).origin;
			};
			T* operator--(){
				node = (*node).prev;
				return (*node).origin;
			};
		private:
			Lock::Key<L> key;
			Node* node;
			Node* const anchor;
		};

		void Add(Lock::Key<L>&, Node& n){
			n.Add(anchor, *this);
		};
		void Add(Node& n){
			Lock::Key<L> k;
			Add(k, n);
		};
		void Insert(Lock::Key<L>&, Node& n){
			n.Insert(anchor, *this);
		};
		void Insert(Node& n){
			Lock::Key<L> k;
			Insert(k, n);
		};

		bool Is(Lock::Key<L>&){
			return anchor.next != &anchor;
		};
		T* Get(Lock::Key<L>& k){
			if(!Is(k)){
				return;
			}
			T* const t((*anchor.next).origin);
			(*anchor.next).Detach(k);
			return t;
		};

	private:
		Node anchor;
	};

}

