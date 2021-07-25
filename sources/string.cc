/********************************************************************** 文字列
 * Copyright (C) 2017,2019,2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/string.h>
#include <toolbox/exception/exception.h>
#include <string.h>



namespace TB {

	String::String(const String& t) { *this = t; }


	String::String(const char* t) { *this = t; }

	String::String(long long value, unsigned length, char padding) : length(0) {
		if (value < 0) {
			*this += '-';
			value = -value;
		}
		FromNumeric(static_cast<unsigned long long>(value), length, padding);
	}
	String::String(unsigned long long value, unsigned length, char padding)
		: length(0) {
		FromNumeric(value, length, padding);
	}

	void String::FromNumeric(
		long long unsigned value,
		unsigned length,
		char padding,
		unsigned radix) {
		// parametor check
		if (radix < 2 || 16 < radix) {
			throw Exception();
		}

		static const unsigned blen(64);

		// pad if length is too long
		for (; blen - 1 < length; --length) {
			*this += padding;
		}

		// build buffer
		char b[blen];
		char* p(&b[blen - 1]);
		*p-- = 0;
		char* p0(&p[-length]);
		if (length) {
			memset(p0, padding, length);
		}
		*p = '0';

		// convert from value
		static const char* const numeric = "0123456789abcdef";
		for (; value; *p-- = numeric[value % radix], value /= radix)
			;

		*this += p < p0 ? p : p0;
	}


	String& String::operator=(const String& t) {
		length = t.length;
		if (length) {
			Resize(length + 1); //+1はゼロ終端の分
			memcpy(&(*this)[0], t.Raw(), length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator=(const char* t) {
		length = t ? strlen(t) : 0;
		if (length) {
			Resize(length + 1);
			memcpy(&(*this)[0], t, length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}

	String& String::operator+=(const String& t) {
		if (!t.length) {
			return *this;
		}
		const unsigned newLength(length + t.length);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t.Raw(), t.length + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator+=(const char* t) {
		if (!t) {
			return *this;
		}
		const unsigned orgLength(strlen(t));
		if (!orgLength) {
			return *this;
		}
		const unsigned newLength(length + orgLength);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t, orgLength + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator+=(char c) {
		char s[2] = {c, 0};
		*this += s;
		return *this;
	}
	String String::operator+(const String& t) const {
		String newString(*this);
		newString += t;
		return newString;
	}
	String String::operator+(const char* t) const {
		String newString(*this);
		newString += t;
		return newString;
	}

	bool String::operator==(const char* t) const { return !strcmp(Raw(), t); }

	String String::SubStr(unsigned b, unsigned l) const {
		String newString;

		if (!l || length <= b) {
			//結果が空
			return newString;
		}

		if (length < b + l) {
			//長さ調整
			l = length - b;
		}

		newString.Resize(l + 1);
		newString.length = l;
		memcpy(&newString[0], Raw() + b, l);
		newString[l] = 0;
		return newString;
	}

	String& String::operator<<(long long unsigned n) {
		FromNumeric(n, 0, ' ', 10);
		return *this;
	}

	String& String::operator<<(long long i) {
		if (i < 0) {
			*this += '-';
			i = -i;
		}
		FromNumeric(static_cast<unsigned>(i), 0, ' ', 10);
		return *this;
	}

}
