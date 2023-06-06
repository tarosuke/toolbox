/* Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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

#include <stdlib.h>

#include <toolbox/container/table.h>



namespace TB {

	Table::Table()
		: table((Entry*)malloc(sizeof(Entry) * initialEntries)),
		  nEntry(initialEntries), pool(emptyEntry) {
		// 確保した領域をpoolに詰んでおく
		for (unsigned n(0); n < nEntry; ++n) {
			table[n].next = pool;
			pool = n;
		}
	}

	Table::~Table() {
		if (table) {
			free(table);
		}
	}

	unsigned Table::Register(Node& node) {
		if (pool == emptyEntry) {
			// メモリを追加確保して新気分をpoolに詰む
			if (!(table = (Entry*)realloc(table, nEntry * sizeof(Entry) * 2))) {
				return 0;
			};
			for (unsigned n(nEntry); n < nEntry * 2; ++n) {
				table[n].next = pool;
				pool = n;
			}
		}

		Entry& ne(table[pool]);
		const unsigned id(pool);
		pool = ne.next;

		ne.node = &node;
		return id;
	}

	Table::Node::~Node() {
		table.table[id].next = table.pool;
		table.pool = id;
	}
}
