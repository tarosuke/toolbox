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

	struct TimeBase : public Type<u64> {
		TimeBase(){};
		void Update(); // 現在時刻取得

	protected:
		TimeBase(u64 ns) : Type<u64>(ns){};
	};

	template <unsigned S> struct Time : public TimeBase {
		Time(){};
		Time(u64 init) : TimeBase(init * S){};
		operator u64() const { return body / S; };
		const Time& operator=(const ::TB::Type<u64>& t) {
			*this = t;
			return *this;
		};
	};

	using sec = Time<1000000000>;
	using msec = Time<1000000>;
	using usec = Time<1000>;
	using nsec = Time<1>;


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
