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
#pragma once



namespace TB {

	struct TableBase {
		static constexpr unsigned initialEntries = 16;

		struct Node {
			virtual ~Node();

		protected:
			TableBase& table;
			const unsigned id;

			Node(TableBase& table) : table(table), id(table.Register(*this)){};
			Node(TableBase& table, unsigned id) : table(table), id(id){};
		};

	protected:
		TableBase();
		~TableBase();

		Node* Find(unsigned id) { return table[id].node; };

	private:
		static constexpr unsigned emptyEntry = ~0L;

		union Entry {
			Node* node;
			unsigned next; // tableはreallocされるのでポインタではなく添字
		};
		Entry* table; // 確保された領域
		unsigned nEntry; // tableに確保されているエントリ数
		unsigned pool; // プールスタックのトップを指す(emptyEntryなら空)

		unsigned Register(Node&); // 新規エントリに引数を登録してIDを設定する
		void Unregister(Node&); // エントリを返却する
	};

	template <typename T> struct Table : TableBase {
		T* operator[](unsigned id) { return dynamic_cast<T*>(Find(id)); };
	};
}
