/************************************************************************* time
 * 時間関係のインフラ
 * Copyright (C) 2021, 2022 tarosuke<webmaster@tarosuke.net>
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
#include "toolbox/type.h"



namespace TB {

	/***** 時間関連クラスの基本
	 * 正直なところ、これらのクラスを作ったのはtimespecを直接触りたくないから。
	 * 十進ベースでバッチイことこの上ない。
	 */
	struct nsec : public Type<u64> {
		nsec() { Update(); };
		nsec(u64 o) : Type(o){};
		const nsec& operator=(const Type<u64>& t) {
			*(Type<u64>*)this = t;
			return *this;
		};
		void Update();
	};

	template <unsigned S> struct Time : nsec {
		Time(){};
		Time(u64 init) : nsec(init * S){};
		operator u64() const { return body / S; };
	};

	using sec = Time<1000000000>;
	using msec = Time<1000000>;
	using usec = Time<1000>;


	// タイムスタンプ管理
	struct Timestamp {
		Timestamp() : uptime(1), delta(1){};
		void Update();
		nsec uptime;
		nsec delta;

	private:
		nsec start;
	};
}
