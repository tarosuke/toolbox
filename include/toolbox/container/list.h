/************************************************************************ List
 * Copyright (C) 2016 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#include "../lock/key.h"



namespace TB{

	template<
		class T,
		int index = 0,
		class L=Lock::NullLock> class List : public L{
		List(const List&);
		void operator=(const List&);
	public:
		//このリストのためのKey(このリストを初期値にして作る)
		typedef Lock::Key<L> Key;

		/**ノード：リストのノードはこれを継承しておく必要がある
		 * 複数のリストを使うときはindexを指定した上で多重継承する
		 *
		 * ノードがdeleteされると自動的にリストから外れる
		 * List自体がdeleteされると要素が切り離された上でNotifyListDeletedが呼ばれる
		 * NotifyListDeletedのデフォルトは「何もしない」なので、
		 * Listが唯一の参照であるならNotifyListDeletedで自身をdeleteする必要がある
		 */
		class Node{
			friend class List;
			friend class I;
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
		protected:
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

		/** 反復子
		 * Javaスタイルの反復子。
		 * 順、逆順共通で以下のように使う。
		 * 正：for(I i(list); ++i;) hoge;
		 * 逆：for(I i(list); --i;) huga;
		 * 初期化時は無効要素を指す。
		 * Listは環状なので初期状態から++すれば最初の要素、
		 * --すれば最後の要素を指すようになる。
		 *
		 * 反復子が指している要素をリストから外しても正常動作するようになっている。
		 * NOTE:反復子が有効な状態でノードを追加しないこと
		 */
		class I{
		public:
			I(List& l) :
				key(l),
				node(&l.anchor),
				prev((*node).prev),
				next((*node).next){};
			operator T*(){
				return *node;
			};
			T* operator++(){
				node = next;
				Prepare();
				return *node;
			};
			T* operator--(){
				node = prev;
				Prepare();
				return *node;
			};
		private:
			Key key;
			Node* node;
			Node* prev;
			Node* next;

			void Prepare(){
				prev = (*node).prev;
				next = (*node).next;
			};

		};
		/** コールバックによる反復
		 */
		template<typename U> T* Foreach(bool (T::*handler)(U&), U& param){
			for(I i(*this); ++i;){
				if(((*i).*handler)(param)){
					return i;
				}
			}
			return 0;
		};
		template<typename U> void Foreach(void (T::*handler)(U&), U& param){
			for(I i(*this); ++i;){
				((*i).*handler)(param);
			}
		};
		void Foreach(void (T::*handler)()){
			for(I i(*this); ++i;){
				((*i).*handler)();
			}
		}
		template<typename U> T* Reveach(bool (T::*handler)(U&), U& param){
			for(I i(*this); --i;){
				if(((*i).*handler)(param)){
					return i;
				}
			}
			return 0;
		};
		template<typename U> void Reveach(void (T::*handler)(U&), U& param){
			for(I i(*this); --i;){
				((*i).*handler)(param);
			}
		};
		void Reveach(void (T::*handler)()){
			for(I i(*this); --i;){
				((*i).*handler)();
			}
		};

		/**要素の操作
		 */
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
		List(){};
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

