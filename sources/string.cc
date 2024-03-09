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
