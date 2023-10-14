/** Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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

#include <limits>
#include <tb/key.h>
#include <tb/types.h>
#include <vector>



namespace tb {

	/***** 要素がNodeで管理される配列
	 * TはArray<T>::Nodeの特である必要がある
	 * 排他制御が必要ならLにロックを設定する(規定は何もしないダミーのロック)
	 * NodeをIDを指定して作るとArray上にIDとして設定される
	 * NodeをIDなしで作るとIDが割り当てられて設定される
	 * Arrayがなくなると繋がれている全NodeのNotifyArrayDeletedが呼ばれる
	 * NOTE:ID指定NodeとIDなしNodeをひとつのArrayで混ぜて使わないこと
	 */
	template <class T, class L = NullLock, typename I = uint> struct Array : L {
		struct Node {
			Node() = delete;
			Node(const Node&) = delete;
			void operator=(const Node&) = delete;

			Node(Array& a, I id) : array(a), id(id) { a.Attach(*this, id); };
			Node(Array& a) : array(a), id(a.Attach(*this)){};
			virtual ~Node() { array.Detach(id); };

		protected:
			virtual void NotifyArrayDeleted(){};

		private:
			Array& array;
			const I id;
		};

		Array() : pool(invalid){};
		~Array() {
			for (auto& i : body) {
				if (i.node) {
					i.NotifyArrayDeleted();
				}
			}
		};

		T operator[](I index) { return body[index]; };

	private:
		static constexpr I invalid = std::numeric_limits<I>::max();
		struct Index {
			Index() : node(0){};

			void operator=(Node& n) { node = &n; };
			operator T&() { return *dynamic_cast<T*>(node); };

			Node* node;
			I next;
		};
		std::vector<Index> body;
		I pool;
		void Detach(I index) {
			Key<L> key(*this);

			// 要素をpoolへつなぐ
			Index i(body[index]);
			i.node = 0;
			i.next = pool;
			pool = index;
		};
		void Attach(Node& node, I index) {
			Key<L> key(*this);

			if (body.size() <= index) {
				body.resize(index);
			}

			attach(node, index);
		};
		I Attach(Node& node) {
			Key<L> key(*this);

			// 要素を割り当ててnodeを結びつける
			if (pool == invalid) {
				// 新規割当
				const I l(body.size());
				body.resize(l + 1);
				attacch(node, l);
				return l;
			}

			// リサイクル
			const I l(pool);
			pool = body[pool].next;
			attach(node, l);
			return l;
		};
		void attach(Node& node, I index) {
			// 要素にnodeを結びつける
			body[index].node = &node;
		};
	};
}
