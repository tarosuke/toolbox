/** Copyright (C) 2024 tarosuke<webmaster@tarosuke.net>
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

	template <class T, class L = NullLock, typename ID = uint> struct Table {
		struct Node {
			void Attach(Table& t) {
				table = &t;
				id = t.Attach(*this);
			};
			void Detach() {
				if (table) {
					table.Detach(id);
					table = 0;
				}
			};
			Node() : table(0) {};
			Node(Table& t) { Attach(t); };
			virtual ~Node() { Detach(); };
			virtual void NotifyTableDeleted() {};

		private:
			Table* table;
			ID id;
		};


		Table() : pool(~(ID)0) {};
		T* operator[](ID id) { return (T*)table[id]; };

	private:
		struct N {
			void Attach(T& t) { target = &t; };
			void Detach(ID p) {
				target = 0;
				next = p;
			};

			T() : target(0) {};
			virtual ~N() {
				if (target) {
					target->NotifyTableDeleted();
				}
			};
			operator T*() { return dynamic_cast<T*>(target); };

		private:
			T* target;
			ID next;
		};
		std::vector<N> table;
		ID pool;

		ID Attach(T& target) {
			Key<Table> key(*this);
			if (~pool) {
				// スタックからリサイクル
				const ID id(pool);
				T& t(table[id]);
				pool = t.next;
				t.Attach(target);
				return id;
			} else {
				// スタックが空なので新規割当
				const ID id(table.size());
				table.resize(id + 1);
				table[id].Attach(target);
				return id;
			}
		};
		void Detach(ID i) {
			Key<Table> key(*this);
			table[i].Detach(pool);
			pool = i;
		};
	};
}
