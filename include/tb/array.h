/** Copyright (C) 2023, 2024 tarosuke<webmaster@tarosuke.net>
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
#include <tb/types.h>
#include <vector>

namespace tb {

	/***** 要素がNodeで管理される配列
	 * TはArray<T>::Nodeの特である必要がある
	 * 排他制御が必要ならLにロックを設定する(規定は何もしないダミーのロック)
	 * Arrayがなくなると繋がれている全NodeのNotifyArrayDeletedが呼ばれる
	 */
	template <class T, typename ID = uint, class L = NullLock>
	struct Array : L {
		struct Node {
			void Attach(Array& t, ID i) {
				array = &t;
				id = i;
				t.Attach(*this, i);
			};
			void Detach() {
				if (array) {
					array->Detach(id);
					array = 0;
				}
			};
			Node() : array(0) {};
			Node(Array& t, ID i) { Attach(t, i); };
			virtual ~Node() { Detach(); };
			virtual void NotifyArrayDeleted() {};

			ID GetID() const { return id; };

		private:
			Array* array;
			ID id;
		};

		Array() = default;
		T* operator[](ID id) { return dynamic_cast<T*>(GetNode(id)); };

	private:
		struct N {
			void Attach(Node& t) { target = &t; };
			void Detach() { target = 0; };

			N() : target(0) {};
			virtual ~N() {
				if (target) {
					target->NotifyArrayDeleted();
				}
			};
			operator T*() { return dynamic_cast<T*>(target); };
			void operator=(T& t) { target = &t; };

			Node* target;
		};
		std::vector<N> array;
		void Attach(Node& target, ID id) {
			Key<Array> key(*this);
			if (array.size() <= id) {
				array.resize(id + 1);
			}
			array[id].Attach(target);
		};
		void Detach(ID i) {
			Key<Array> key(*this);
			array[i].Detach();
		};
		Node* GetNode(ID i) const {
			Key<Array> key(*this);
			return array[i].target;
		};
	};
}
