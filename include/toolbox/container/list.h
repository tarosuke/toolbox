/************************************************************************ List
 * Copyright (C) 2017, 2021 tarosuke<webmaster@tarosuke.net>
 */

#pragma once

#include "../lock/key.h"



namespace TB{

	template <class T, class L = Lock::NullLock> class List : public L {
		List(const List&);
		void operator=(const List&);
	public:
		//このリストのためのKey(このリストを初期値にして作る)
		using Key = Lock::Key<L>;

		/**ノード：リストのノードはこれをpublicで継承しておく必要がある
		 * 複数のリストを使うときはそれぞれ継承してxxLisuのようにして区別する
		 *
		 * ノードがdeleteされると自動的にリストから外れる
		 * List自体がdeleteされると要素が切り離された上でNotifyListDeletedが呼ばれる
		 * NotifyListDeletedのデフォルトは「何もしない」なので、
		 * Listが主参照であるならNotifyListDeletedで自身をdeleteする必要がある
		 */
		class Node{
			friend class List;
			friend class I;
			friend class itor;
			Node(const Node&);
			void operator=(const Node&);
		public:
			virtual ~Node(){ Detach(); };
		protected:
			Node(bool deleteWithList=false) :
				prev(this), next(this), deleteWithList(deleteWithList){};
			virtual void NotifyListDeleted(){
				if(deleteWithList){ delete this; } };
		private:
			Node* prev;
			Node* next;
			const bool deleteWithList;
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
				return dynamic_cast<T*>(this);
			};
		};

		/** 反復子
		 * Listを与えればJavaスタイルの反復子。
		 * 順、逆順共通で以下のように使う。
		 * 正：for(I i(list); ++i;) hoge;
		 * 逆：for(I i(list); --i;) huga;
		 * 初期化時は無効要素を指す。
		 * Listは環状なので初期状態から++すれば最初の要素、
		 * --すれば最後の要素を指すようになる。
		 *
		 * Nodeを与えるとその場を指すのでC++スタイルの反復子
		 *
		 * 反復子が指している要素をリストから外しても正常動作するようになっている。
		 * NOTE:反復子が有効な状態で要素を追加するときは反復子のInsert、Addを使うこと
		 */
		class I{
		public:
			// Javaスタイル反復子
			I(List& l) :
				key(l),
				node(&l.anchor),
				prev((*node).prev),
				next((*node).next){};
			// C++スタイル反復子
			I(Node& i)
				: key(i.key), node(&i.node), prev((*node).prev),
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
			void Insert(Node& n){
				n.Insert(*node);
				Prepare();
			};
			void Add(Node& n){
				n.Attach(*node);
				Prepare();
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

		/** 検索
		 * ありがちな[]演算子による検索
		 * T*を返す。なければ0
		 * Tにはキーに対応した型の==演算子が必要
		 * なお、シリアルサーチなのでO(n)、つまり遅い
		 */
		template<typename K> T* operator[](K& key){
			for(I i(*this); ++i;){
				if((*i) == key){
					return i;
				}
			}
			return 0;
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
			return dynamic_cast<T*>(anchor.next);
		};
		T* Bottom(Key&){
			return dynamic_cast<T*>(anchor.prev);
		};

		T* Get(Key&){
			Node* const tn(anchor.next);
			if(tn != &anchor){
				(*tn).Detach();
				return dynamic_cast<T*>(tn);
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

		// C++スタイル反復子生成
		static I begin(List& list) { return I(list.ancher.next); };
		static I end(List& list) { return I(list.ancher); };

	private:
		Node anchor;
	};
}
