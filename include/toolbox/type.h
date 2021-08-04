/***** 基本型のクラスとシノニム
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <cstdint>



namespace TB {

	template <typename T> class Type {
	public:
		Type(){};
		Type(const T& t) : body(t){};
		const T& operator=(const T& t) {
			body = t;
			return body;
		};
		operator const T&() const { return body; };


	protected:
		T body;
	};

}


using u32 = __uint32_t;
using i32 = __int32_t;
using u64 = __uint64_t;
using i64 = __int64_t;
using u128 = __uint128_t;
using i128 = __int128_t;


#define elementsOf(a) (sizeof(a) / sizeof(a[0]))
