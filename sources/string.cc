/********************************************************************** 文字列
 * Copyright (C) 2017,2019,2021,2023 tarosuke<webmaster@tarosuke.net>
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

#include <stdio.h>
#include <tb/string.h>



namespace tb {

	const char String::numericChars[] = "0123456789abcdef";

	template <> String& String::Append(u64 v, char p, uint w, uint r) {
		if (!w) {
			w = 1;
		}

		// 最後の文字
		String s(numericChars[v % r]);

		// 剰余を並べて
		for (; v /= r, (w ? --w : w) || v;) {
			s += numericChars[v % r];
		}
		// 逆順に追加
		for (auto i(s.crbegin()); i != s.crend(); ++i) {
			push_back(*i);
		}

		return *this;
	}
	template <> String& String::Append(u8 v, char p, uint w, uint r) {
		return Append((u64)v, p, w, r);
	}
	template <> String& String::Append(u16 v, char p, uint w, uint r) {
		return Append((u64)v, p, w, r);
	}
	template <> String& String::Append(u32 v, char p, uint w, uint r) {
		return Append((u64)v, p, w, r);
	}
	template <> String& String::Append(i64 v, char p, uint w, uint r) {
		if (!w) {
			w = 1;
		}
		if (v < 0) {
			push_back('-');
			Append((uint)-v, p, w - 1, r);
		} else {
			Append((uint)v, p, w, r);
		}
		return *this;
	}
	template <> String& String::Append(i8 v, char p, uint w, uint r) {
		return Append((i64)v, p, w, r);
	}
	template <> String& String::Append(i16 v, char p, uint w, uint r) {
		return Append((i64)v, p, w, r);
	}
	template <> String& String::Append(i32 v, char p, uint w, uint r) {
		return Append((i64)v, p, w, r);
	}
	template <> String& String::Append(double v, char p, uint w, uint r) {
		char b[std::numeric_limits<double>::max_digits10];
		snprintf(b, sizeof(b), "%*f", w, v);
		append(b);
		return *this;
	}
	template <> String& String::Append(float v, char p, uint w, uint r) {
		return Append((double)v, p, w, r);
	}
}



#include <string.h>
#include <toolbox/exception/exception.h>
#include <toolbox/string.h>



namespace TB {

	const char* const String::numeric = "0123456789abcdef";

	String::String(const String& t) { *this = t; }
	String::String(const char* t) { Load(t); }

	String& String::operator<<(const String& t) {
		Copy(t, Length());
		return *this;
	}
	String& String::operator<<(const char* t) {
		unsigned i(Length());
		for (; t && *t; ++t, ++i) {
			Copy(*t, i);
		}
		Copy(0, i);
		return *this;
	}
	String& String::operator<<(char c) {
		Resize(elements + 1);
		body[elements - 2] = c;
		body[elements - 1] = 0;
		return *this;
	}

	void String::FromSigned(
		long long v, unsigned minLen, char padding, unsigned radix) {
		if (v < 0) {
			*this << '-';
			if (minLen) {
				--minLen;
			}
			FromUnsigned(-v, minLen, padding, radix);
		}
	}

	void String::FromUnsigned(
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
			*this << padding;
		}

		// build buffer
		char b[blen];
		char* p(&b[blen - 1]);
		*p-- = 0;
		char* const p0(&p[-length]); // pの初期位置からlength戻した位置

		// convert from value
		do {
			*p-- = numeric[value % radix], value /= radix;
		} while (value);

		if (p0 < ++p) {
			memset(p0, padding, p - p0 - 1);
			p = p0;
		}
		*this << p;
	}


	String& String::operator=(const char* t) {
		Load(t);
		return *this;
	}

	void String::Load(const char* t) {
		Resize(0);
		unsigned i(0);
		for (; t && *t; ++t, ++i) {
			Copy(*t, i);
		}
		Copy(0, i);
	}

	String String::operator+(const String& t) const {
		String newString(*this);
		newString << t;
		return newString;
	}
	String String::operator+(const char* t) const {
		String newString(*this);
		newString << t;
		return newString;
	}

	bool String::operator==(const char* t) const { return !strcmp(body, t); }


	Array<String> String::Split(const char* delimitor) const {
		Array<String> arr;
		if (!delimitor) {
			arr += *this;
			return arr;
		}
		String newString;
		for (const char* c(*this); c && *c; ++c) {
			if (*delimitor == *c) {
				// デリミタチェック
				const char* d(delimitor + 1);
				const char* e(++c);
				for (; *d && *e && *d == *e; ++d, ++e) {}
				if (!*d) {
					// デリミタ確認
					arr += newString;
					newString.Clear();
					c = e - 1;
				} else {
					// すでにcを一つ進めてあるので代わりを追加
					newString << *delimitor;
				}
			} else {
				newString << *c;
			}
		}
		arr += newString;
		return arr;
	}
}
