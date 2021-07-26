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

	String::String(long long value, unsigned length, char padding) {
		if (value < 0) {
			*this += '-';
			value = -value;
		}
		FromNumeric(static_cast<unsigned long long>(value), length, padding);
	}
	String::String(unsigned long long value, unsigned length, char padding) {
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


	String& String::operator=(const char* t) {
		Resize(0);
		for (; t && *t; ++t) {
			*this += *t;
		}
		Append(0);
		return *this;
	}

	String& String::operator+=(const String& t) {
		CutTail();
		Append(t);
		return *this;
	}
	String& String::operator+=(const char* t) {
		CutTail();
		for (; t && *t; ++t) {
			Append(*t);
		}
		Append(0);
		return *this;
	}
	String& String::operator+=(char c) {
		CutTail();
		Append(c);
		Append(0);
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

	bool String::operator==(const char* t) const {
		return !strncmp(body, t, Length());
	}


	String& String::operator<<(long long unsigned n) {
		FromNumeric(n, 0, ' ', 10);
		return *this;
	}

	String& String::operator<<(long long i) {
		if (i < 0) {
			Append('-');
			i = -i;
		}
		FromNumeric(static_cast<unsigned>(i), 0, ' ', 10);
		return *this;
	}

}
