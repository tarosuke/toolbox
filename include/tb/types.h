/***** 基本型の短縮形
 * Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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

#include <stdint.h>

namespace tb {

	using u8 = uint8_t;
	using i8 = int8_t;
	using u16 = uint16_t;
	using i16 = int16_t;
	using u32 = uint32_t;
	using i32 = int32_t;
	using u64 = uint64_t;
	using i64 = int64_t;
	using u128 = unsigned __int128;
	using i128 = __int128;
	using f32 = float;
	using f64 = double;
	using f128 = long double;

	using uint = unsigned;

	// クラス化(Tは基本型を想定している)
	template <typename T> struct Class {
		Class() = default;
		Class(const T& t) : body(t) {};
		T& operator=(const T& t) { return body = t; };
		operator T() const { return body; };

		// NOTE:二項演算子は既存のそれと重複する可能性があるので暫定的に削除
		// bool operator==(const T& t) const { return body == t; };
		// bool operator!=(const T& t) const { return body != t; };

		// T operator+(const T& t) const { return body + t; };
		// T operator-(const T& t) const { return body - t; };
		// T operator*(const T& t) const { return body * t; };
		// T operator/(const T& t) const { return body / t; };
		// T operator%(const T& t) const { return body % t; };
		// T operator&(const T& t) const { return body & t; };
		// T operator|(const T& t) const { return body | t; };
		// T operator^(const T& t) const { return body ^ t; };
		// T operator<<(const T& t) const { return body << t; };
		// T operator>>(const T& t) const { return body >> t; };
		// T operator~() const { return ~body; };

		T& operator+=(const T& t) { return body += t; };
		T& operator-=(const T& t) { return body -= t; };
		T& operator*=(const T& t) { return body *= t; };
		T& operator/=(const T& t) { return body /= t; };
		T& operator%=(const T& t) { return body %= t; };
		T& operator&=(const T& t) { return body &= t; };
		T& operator|=(const T& t) { return body |= t; };
		T& operator<<=(const T& t) { return body <<= t; };
		T& operator>>=(const T& t) { return body >>= t; };

	protected:
		T body;
	};
}

#define elementsOf(a) (sizeof(a) / sizeof(a[0]))
