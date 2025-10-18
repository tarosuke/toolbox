/************************************************************************ List
 * Copyright (C) 2017, 2021, 2023 tarosuke<webmaster@tarosuke.net>
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

#include <tb/key.h>



namespace tb {

	template <class T, class L = NullLock> struct List : private L {
		List(const List&) = delete;
		void operator=(const List&) = delete;

		/**ノード：リストのノードはこれをpublicで継承しておく必要がある
		 * 複数のリストを使うときはそれぞれ継承してxxListのような子を作って
		 * 区別する
		 *
		 * ノードがdeleteされると自動的にリストから外れる
		 * またList自体がdeleteされると要素が切り離された上でNotifyListDeletedが
		 * 呼ばれる
		 */
		class Node {
			friend class List;
			friend class I;
			friend class itor;
			Node(const Node&);
			void operator=(const Node&);

		public:
			virtual ~Node() { Detach(); };

		protected:
			Node() : prev(this), next(this) {};
			virtual void NotifyListDeleted() {};

		private:
			Node* prev;
			Node* next;

		protected:
			void Insert(Node& n) {
				if (prev != this) {
					Detach();
				}
				prev = n.prev;
				next = &n;
				(*prev).next = n.prev = this;
			};
			void Attach(Node& n) {
				if (prev != this) {
					Detach();
				}
				next = n.next;
				prev = &n;
				(*next).prev = n.next = this;
			};
			void Detach() {
				(*prev).next = next;
				(*next).prev = prev;
				prev = next = this;
			};
		};

		/** toolbox反復子
		 * 順、逆順共通で以下のように使う。
		 * 正：for(I i(list); ++i;) hoge;
		 * 逆：for(I i(list); --i;) huga;
		 * 初期化時は無効要素を指す。
		 * Listは環状なので初期状態から++すれば最初の要素、
		 * --すれば最後の要素を指すようになる。
		 * NOTE: なお、反復子は前後の情報を保持しているので要素を削除することも
		 *      可能だが、反復子が有効な状態で要素を追加するときはリストではなく
		 *      反復子のInsert/Addを使う
		 */
		class I {
		public:
			// toolboxスタイル反復子
			I(List& l) :
				key(l),
				node(&l.anchor),
				prev((*node).prev),
				next((*node).next) {};
			operator T*() { return dynamic_cast<T*>(node); };
			T& operator*() { return *dynamic_cast<T*>(node); };
			T& operator->() { return *dynamic_cast<T*>(node); };
			T* operator++() {
				node = next;
				Prepare();
				return dynamic_cast<T*>(node);
			};
			T* operator--() {
				node = prev;
				Prepare();
				return dynamic_cast<T*>(node);
			};
			void Insert(Node& n) {
				n.Insert(*node);
				Prepare();
			};
			void Add(Node& n) {
				n.Attach(*node);
				Prepare();
			};

		private:
			Key<L> key;
			Node* node;
			Node* prev;
			Node* next;

			void Prepare() {
				prev = (*node).prev;
				next = (*node).next;
			};
		};

		/** コールバックによる反復
		 */
		template <typename... U>
		T* Foreach(bool (T::*handler)(U...), U&&... params) {
			for (I i(*this); ++i;) {
				if (((*i).*handler)(params...)) {
					return i;
				}
			}
			return 0;
		};
		template <typename... U>
		void Foreach(void (T::*handler)(U...), U&&... params) {
			for (I i(*this); ++i;) {
				(i->*handler)(params...);
			}
		};
		template <typename... U>
		T* Reveach(bool (T::*handler)(U...), U&&... params) {
			for (I i(*this); --i;) {
				if ((i->*handler)(params...)) {
					return i;
				}
			}
			return 0;
		};
		template <typename... U>
		void Reveach(void (T::*handler)(U...), U&&... params) {
			for (I i(*this); --i;) {
				(i->*handler)(params...);
			}
		};

		/** 検索
		 * ありがちな[]演算子による検索
		 * 発見できたらT*を返す。なければ0
		 * Tにはキーに対応した型の==演算子が必要
		 * なお、シリアルサーチなのでO(n)、つまり遅い
		 */
		template <typename K> T* operator[](K& key) {
			for (I i(*this); ++i;) {
				if ((*i) == key) {
					return i;
				}
			}
			return 0;
		};

		/**要素の操作
		 */
		void Add(Key<L>&, Node& n) { n.Insert(anchor); };
		void Add(Node& n) {
			Key<L> k(*this);
			Add(k, n);
		};
		void Insert(Key<L>&, Node& n) { n.Attach(anchor); };
		void Insert(Node& n) {
			Key<L> k(*this);
			Insert(k, n);
		};
		T* Top(Key<L>&) const { return dynamic_cast<T*>(anchor.next); };
		T* Bottom(Key<L>&) const { return dynamic_cast<T*>(anchor.prev); };
		T* Top() const {
			Key<L> k(*this);
			return Top(k);
		};
		T* Botom() const {
			Key<L> k(*this);
			return Bottom(k);
		};

		T* Get(Key<L>&) {
			Node* const tn(anchor.next);
			if (tn != &anchor) {
				(*tn).Detach();
				return dynamic_cast<T*>(tn);
			}
			return 0;
		};
		T* Get() {
			Key<L> key(*this);
			return Get(key);
		};

		// コンストラクタ／デストラクタ
		List() {};
		~List() {
			while (anchor.next != &anchor) {
				Node* const n(anchor.next);
				if (n == &anchor) {
					continue;
				}
				(*n).Detach();
				(*n).NotifyListDeleted();
			}
		};

		// C++スタイル反復子
		struct itor {
			itor(Node& n) : node(&n) {};
			T* operator*() { return dynamic_cast<T*>(node); };
			bool operator==(const itor& t) const { return node == t.node; };
			bool operator!=(const itor& t) const { return node != t.node; };
			itor operator++() {
				itor i(*node);
				node = (*node).next;
				return i;
			};
			itor operator++(int) {
				node = (*node).noxt;
				return *this;
			};

		private:
			Node* node;
		};
		itor begin() { return itor(*anchor.next); };
		itor end() { return itor(anchor); };
		itor rbegin() { return itor(*anchor.prev); };
		itor lend() { return itor(anchor); };

	private:
		Node anchor;
	};
}
