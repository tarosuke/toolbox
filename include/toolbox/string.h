/********************************************************************** 文字列
 * Copyright (C) 2017,2019 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/container/array.h>



namespace TB{

	class String : public Array<char>{
	public:
		// manipulators
		class endl;
		class end;
		class hex;

		// constructors
		String() : length(0){ (*this)[0] = 0; };
		String(const String&);
		String(const char*);
		String(
			long long,
			unsigned length = 0,
			char padding = ' ');
		String(
			unsigned long long,
			unsigned length = 0,
			char padding = ' ');

		String& operator=(const String&);
		String& operator=(const char*);

		operator char*(){
			return GetRawBody();
		};
		operator const char*() const {
			return GetRawBody();
		};
		String& operator +=(const String&);
		String& operator +=(const char*);
		String& operator +=(char);
		String operator +(const String&) const;
		String operator +(const char*) const;

		// << operator
		String& operator <<(long long);
		String& operator <<(long long unsigned);
		String& operator <<(const String&);

		bool operator==(const char*) const;

		bool IsEmpty() const { return !length; };
		unsigned Length() const { return length; };

		// string manipulators
		String SubStr(unsigned start, unsigned length)const;
		void Clear(){ (*this)[0] = 0; length = 0; };

	private:
		unsigned radix;
		unsigned length;

		void FromNumeric(
			long long unsigned,
			unsigned length,
			char padding,
			unsigned radix = 10);
	};

}
