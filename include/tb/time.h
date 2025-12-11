/************************************************************************* time
 * 時間関係のインフラ
 * Copyright (C) 2021,2022,2023 tarosuke<webmaster@tarosuke.net>
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
#include "tb/types.h"
#include <sys/time.h>

namespace tb {

	/***** 時間関連クラスの基本
	 * timespecやtimevalを直接触りたくない。
	 * 十進ベースでバッチイことこの上ない。
	 */
	struct nsec : public Class<i64> {
		nsec() { Update(); };
		nsec(u64 o) : Class(o) {};
		const nsec& operator=(const nsec& t) {
			*(Class*)this = t;
			return *this;
		};
		void Update();
		operator timeval() {
			struct timeval t = {
				tv_sec : *this / 1000000000L,
				tv_usec : (*this % 1000000000L) / 1000L
			};
			return t;
		};
		operator timespec() {
			struct timespec t = {
				tv_sec : *this / 1000000000LL,
				tv_nsec : *this % 1000000000LL
			};
			return t;
		};
	};

	template <unsigned S> struct Time : nsec {
		Time() {};
		Time(i64 init) : nsec(init * S) {};
		const Time& operator=(i64 t) { body = t * S; };
		operator i64() const { return body / S; };

		bool operator==(const nsec& t) const { return body == t * S; };
		bool operator!=(const nsec& t) const { return body != t * S; };
	};

	using sec = Time<1000000000>;
	using msec = Time<1000000>;
	using usec = Time<1000>;

	// タイムスタンプ管理
	struct Timestamp {
		Timestamp() : uptime(1), delta(1) {};
		void Update();
		nsec Uptime() const { return uptime; };
		nsec Delta() const { return delta; };

		nsec operator-(Timestamp t) const { return uptime - t.uptime; };

	private:
		nsec start;
		nsec uptime;
		nsec delta;
	};
}
