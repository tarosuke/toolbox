/**************************************************************** instanceList
 * Copyright (C) 2020,2023 tarosuke<webmaster@tarosuke.net>
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
 *
 * タイプ別インスタンスリスト
 * インスタンスリストを使いたいクラスの親にLinstanceList::Nodeを追加する
 * NOTE: listをどこかに置くこと
 */
#pragma once

#include "list.h"



namespace TB {

	template <class T> class InstanceList {
	public:
		class Node : private List<InstanceList::Node> {
			Node(const Node&);
			void operator=(const Node&);

		public:
			Node() { InstanceList::list.Add(*this); };
		};

	protected:
		static List<InstanceList::Node> list;
	};

}
