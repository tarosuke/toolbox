/* Copyright (C) 2014, 2023 tarosuke<webmaster@tarosuke.net>
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

#include <type_traits>



namespace tb {

	/***** RAIIキー
	 * キーが存在する間はロックされる
	 */
	template <class L> struct Key {
		Key(const L& l) : lock(l) { l.Lock(); };
		~Key() { lock.Unlock(); };

	private:
		const L& lock;
	};

	/***** 何もしないロック
	 */
	struct NullLock {
		friend class Key<NullLock>;
		void Lock() const {};
		void Unlock() const {};
	};
};
