/************************************************************************* time
 * 時間関係のインフラ
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
#include "toolbox/type.h"



namespace TB {

	struct TimeBase : public Type<u64> {
		TimeBase();

	protected:
		TimeBase(u64 ns) : Type<u64>(ns){};
		TimeBase(const Type<u64>& t) : Type<u64>(t){};
	};

	template <unsigned S> struct Time : public TimeBase {
		Time(){};
		Time(u64 init) : TimeBase(init * S){};
		Time(const Type<u64>& t) : TimeBase(t){};
		operator u64() const { return body / S; };
	};

	using sec = Time<1000000000>;
	using msec = Time<1000000>;
	using usec = Time<1000>;
	using nsec = Time<1>;

}
