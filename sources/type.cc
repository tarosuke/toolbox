/***** 基本型のクラスとシノニム
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
#include <stdlib.h>
#include <ctype.h>

#include <toolbox/type.h>



namespace {

	const char* const num = "0123456789abcdef";

	char* UDec(u128 v, char b[64]) {
		char* p(&b[63]);
		*p-- = 0;
		do {
			*p-- = num[v % 10];
			v /= 10;
		} while (v);
		return p + 1;
	}
	char* Dec(i128 v, char b[64]) {
		char* p(UDec((u128)v, b));
		if (0 <= v) {
			return p;
		}
		*--p = '-';
		return p;
	}
	// char* Hex(u128 v, char b[64]){
	// 	char* p(&b[63]);
	// 	*p-- = 0;
	// 	do{
	// 		*p-- = num[v & 0xf];
	// 		v >>= 4;
	// 	}while(v);
	// 	*p = 'x';
	// 	*--p = '0';
	// 	return p;
	// }
}

namespace TB {

	template <>
	const String& Stringable<String>::operator=(const char* text) {
		body = text;
		return body;
	}

	template <> const int& Stringable<int>::operator=(const char* text) {
		body = atoi(text);
		return body;
	}
	template <> const long& Stringable<long>::operator=(const char* text) {
		body = strtol(text, 0, 0);
		return body;
	}
	template <>
	const long long& Stringable<long long>::operator=(const char* text) {
		body = strtoll(text, 0, 0);
		return body;
	}
	template <>
	const unsigned& Stringable<unsigned>::operator=(const char* text) {
		body = strtoul(text, 0, 0);
		return body;
	}
	template <> const unsigned long&
	Stringable<unsigned long>::operator=(const char* text) {
		body = strtoul(text, 0, 0);
		return body;
	}
	template <> const unsigned long long&
	Stringable<unsigned long long>::operator=(const char* text) {
		body = strtoull(text, 0, 0);
		return body;
	}

	template <> const float& Stringable<float>::operator=(const char* text) {
		if (text[0] == '0' && text[1] == 'x') {
			//ビットイメージで読み込み
			union {
				u32 raw;
				float val;
			} u;
			u.raw = strtoul(text, 0, 0);
			body = u.val;
		} else {
			body = strtof(text, 0);
		}
		return body;
	}
	template <>
	const double& Stringable<double>::operator=(const char* text) {
		if (text[0] == '0' && text[1] == 'x') {
			//ビットイメージで読み込み
			union {
				u64 raw;
				float val;
			} u;
			u.raw = strtoull(text, 0, 0);
			body = u.val;
		} else {
			body = strtod(text, 0);
		}
		return body;
	}
	template <>
	const long double& Stringable<long double>::operator=(const char* text) {
		if (text[0] == '0' && text[1] == 'x') {
			//ビットイメージで読み込み
			text += 2;
			u128 v(0);
			for (; *text && isxdigit(*text); ++text) {
				v <<= 16;
				v += isdigit(*text) ? *text - '0' : (*text & ~0x20) - 'A' + 10;
			}

			union {
				u128 raw;
				long double val;
			} u;
			u.raw = v;
			body = u.val;
		} else {
			body = strtold(text, 0);
		}
		return body;
	}


	template <> String Stringable<String>::Serialize() const { return body; }
	template <> String Stringable<int>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template <> String Stringable<uint>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	template <> String Stringable<long>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template <> String Stringable<unsigned long>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	template <> String Stringable<long long>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template <> String Stringable<unsigned long long>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	// template<> String Stringable<float>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u32*)&body, b));
	// }
	// template<> String Stringable<double>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u64*)&body, b));
	// }
	// template<> String Stringable<long double>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u128*)&body, b));
	// }
}
