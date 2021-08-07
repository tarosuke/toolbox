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
#pragma once

#include <inttypes.h>
#include <string.h>
#include <math.h>

#include <toolbox/type.h>
#include <toolbox/container/array.h>



namespace TB {

	class String : public Array<char> {
	public:
		// manipulators
		class endl;
		class end;
		class hex;

		// constructors
		String() {
			Resize(1);
			body[0] = 0;
		};
		String(const String&);
		String(const char*);
		String(long long, unsigned length = 0, char padding = ' ');
		String(unsigned long long, unsigned length = 0, char padding = ' ');

		String& operator=(const char*);

		// 暗黙変換
		operator char*() { return Raw(); };
		operator const char*() const { return Raw(); };

		// 明示的変換
		u32 ToU(int radix = 10) const { return strtoul(body, 0, radix); };
		i32 ToI(int radix = 10) const { return strtol(body, 0, radix); };
		u64 ToU64(int radix = 10) const { return strtoull(body, 0, radix); };
		i64 ToI64(int radix = 10) const { return strtoll(body, 0, radix); };
		f32 ToF() const { return strtof(body, 0); };
		f64 ToF64() const { return strtod(body, 0); };
		f128 ToF128() const { return strtold(body, 0); };

		// 何らかの文字列追加
		String& operator+=(const String&);
		String& operator+=(const char*);
		String& operator+=(char);
		String operator+(const String&) const;
		String operator+(const char*) const;

		// << operator
		String& operator<<(long long);
		String& operator<<(long long unsigned);
		String& operator<<(const String&);

		// C文字列と比較
		bool operator==(const char*) const;
		bool operator!=(const char* t) const { return !(*this == t); };

		// 内部状態取得
		bool IsEmpty() const { return Array::Length() <= 1; };
		unsigned Length() const { return Array::Length() - 1; };

		// デリミタで分割
		Array<String> Split(const char* delimiter) const;

		// クリア
		void Clear() {
			Resize(1);
			(*this)[0] = 0;
		};

	private:
		static const char* const numeric;
		void FromNumeric(
			long long unsigned,
			unsigned length,
			char padding,
			unsigned radix = 10);
		void Load(const char*);
	};
}
