/* Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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

#include <string>
#include <tb/image.h>
#include <tb/types.h>

namespace tb {

	// std::stringの拡張
	struct String : public ::std::string {
		String() = default;

		// std::stringへ丸投げして構築＆複製＆移動
		String(const String& s) : ::std::string(s) {};
		String(String&& s) : ::std::string(s) {};
		String(const char* s) : ::std::string(s) {};
		String(char c) : ::std::string(1, c) {};

		String& operator=(const String& s) {
			*(::std::string*)this = s;
			return *this;
		};
		String& operator=(String&& s) {
			*(::std::string*)this = s;
			return *this;
		};
		String& operator=(const char* s) {
			*(::std::string*)this = s;
			return *this;
		};

		// 定義することで何でもtb:;Stringへ変換できるようにする
		template <typename T> String(const T&);
		template <typename T> String& operator=(const T&);
		template <typename T> String& operator=(T&&);

		String operator+(const String& t) const {
			String r(*this);
			r += t;
			return r;
		};

		// const char*へのキャスト
		operator const char*() const { return c_str(); };

		// 文字列に数値文字列を追加(appendでないのはstd:;stringとの区別)
		template <typename T>
		String& Append(T, char padding = ' ', uint width = 1, uint radix = 10);

	private:
		static const char numericChars[];
	};
}
