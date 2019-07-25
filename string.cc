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

#include "include/toolbox/string.h"
#include <string.h>



namespace TB{

	unsigned String::radix(10);


	String::String(const String& t){
		*this = t;
	}


	String::String(const char* t){
		*this = t;
	}


	String& String::operator=(const String& t){
		length = t.length;
		if(length){
			Resize(length + 1); //+1はゼロ終端の分
			memcpy(&(*this)[0], t.GetRawBody(), length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator=(const char* t){
		length = t ? strlen(t) : 0;
		if(length){
			Resize(length + 1);
			memcpy(&(*this)[0], t, length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}

	String& String::operator+=(const String& t){
		if(!t.length){ return *this; }
		const unsigned newLength(length + t.length);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t.GetRawBody(), t.length + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator+=(const char* t){
		if(!t){ return *this; }
		const unsigned orgLength(strlen(t));
		if(!orgLength){ return *this; }
		const unsigned newLength(length + orgLength);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t, orgLength + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator+=(char c){
		char s[2] = { c, 0 };
		*this += s;
		return *this;
	}
	String String::operator+(const String& t)const{
		String newString(*this);
		newString += t;
		return newString;
	}
	String String::operator+(const char* t)const{
		String newString(*this);
		newString += t;
		return newString;
	}

	bool String::operator==(const char* t)const{
		return !strcmp(GetRawBody(), t);
	}

	String String::SubStr(unsigned b, unsigned l)const{
		String newString;

		if(!l || length <= b){
			//結果が空
			return newString;
		}

		if(length < b + l){
			//長さ調整
			l = length - b;
		}

		newString.Resize(l + 1);
		newString.length = l;
		memcpy(&newString[0], GetRawBody() + b, l);
		newString[l] = 0;
		return newString;
	}

	String& String::operator<<(unsigned n){
		static const char* const num("0123456789abcdef");
		char b[16];
		char* t(&b[14]);
		for(b[15] = 0, *t = '0'; n; *t-- = num[n % radix], n /= radix);
		*this += t;
		radix = 10;
		return *this;
	}

	String& String::operator<<(int i){
		if(i < 0){
			*this += '-';
			i = -i;
		}
		return *this << static_cast<unsigned>(i);
	}

}
