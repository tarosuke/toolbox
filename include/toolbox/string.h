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
		// 廃止
		template <typename T> void operator+=(const T&) = delete;

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
		template <typename U> String(const U&);

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

		// 文字、文字列追加
		String operator+(const String&) const;
		String operator+(const char*) const;

		// 文字、文字列への追加
		String& operator<<(const String& t);
		String& operator<<(const char* t);
		String& operator<<(char c);
		template <typename T>
		typename std::enable_if<std::is_integral<T>::value, String&>::type
		operator<<(T v) {
			From(v);
			return *this;
		};
		template <typename T>
		typename std::enable_if<std::is_unsigned<T>::value>::type From(
			T v, unsigned minLen = 0, char padding = ' ', unsigned radix = 10) {
			FromUnsigned(v, minLen, padding, radix);
		};
		template <typename T>
		typename std::enable_if<std::is_signed<T>::value>::type From(
			T v, unsigned minLen = 0, char padding = ' ', unsigned radix = 10) {
			FromSigned(v, minLen, padding, radix);
		};

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
		void Load(const char*);

		void FromSigned(
			long long value, unsigned minLen, char padding, unsigned radix);

		void FromUnsigned(
			long long unsigned value,
			unsigned minLen,
			char padding,
			unsigned radix);
	};
}
