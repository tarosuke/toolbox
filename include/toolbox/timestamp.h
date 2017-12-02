/** タイムスタンプ
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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


namespace TB{

	class Timestamp{
	public:
		template<long long s, long long ns> class time{
		public:
			time(){};
			time(const Timestamp& t) : value(t.sec * s + t.nsec / ns){};
			void operator=(const Timestamp& t){ value = t.sec * s + t.nsec / ns; };
			time& operator+=(const time& t){ value += t.value; return *this; };
			time& operator-=(const time& t){ value -= t.value; return *this; };
			time& operator*=(long long t){ value *= t; return *this; };
			time& operator/=(long long t){ value = t; return *this; };
			time operator+(const time& t) const { time r(*this); r+= t; return r; };
			time operator-(const time& t) const { time r(*this); r-= t; return r; };
			time operator*(long long t) const { time r(*this); r*= t; return r; };
			time operator/(long long t) const { time r(*this); r/= t; return r; };
			operator long long(){ return value; };
		private:
			long long value;
		};
		using ms = time<1000LL, 1000000LL>;
		using us = time<1000000LL, 1000LL>;
		using ns = time<1000000000LL, 1LL>;
		operator double(){ return (double)sec + 0.000000001f * nsec; };

		Timestamp();
	private:
		long sec;
		long nsec;
	};

}
