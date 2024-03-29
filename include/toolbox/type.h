/***** 基本型のクラスとシノニム
 * Copyright (C) 2021, 2023 tarosuke<webmaster@tarosuke.net>
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

#include <cstdint>



namespace TB {

	template <typename T> class Type {
	public:
		Type() = default;
		Type(const T& t) : body(t){};
		const T& operator=(const T& t) {
			body = t;
			return body;
		};
		operator T&() { return body; };

		Type operator+(const Type& t) const { return Type(body + t.body); };
		Type operator-(const Type& t) const { return Type(body - t.body); };
		Type operator+=(const Type& t) {
			body += t.body;
			return *this;
		};
		Type operator-=(const Type& t) {
			body -= t.body;
			return *this;
		};
		template <typename U> Type operator*(U& t) const {
			return Type(body * t);
		};
		template <typename U> Type operator*=(const U& t) {
			body *= t;
			return *this;
		};
		template <typename U> Type operator/(U& t) const {
			return Type(body / t);
		};
		template <typename U> Type operator/=(const U& t) {
			body /= t;
			return *this;
		};

		template <typename U> bool operator<(U& t) const {
			return body < t.body;
		};
		template <typename U> bool operator<=(U& t) const {
			return body <= t.body;
		};
		template <typename U> bool operator>(U& t) const {
			return body > t.body;
		};
		template <typename U> bool operator>=(U& t) const {
			return body >= t.body;
		};

	protected:
		T body;
	};

}


using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using u128 = __uint128_t;
using i128 = __int128_t;
using f32 = float;
using f64 = double;
using f128 = long double;

template <typename T> struct SameSizeInt;
template <> struct SameSizeInt<f32> : public TB::Type<i32> {};
template <> struct SameSizeInt<f64> : public TB::Type<i64> {};
template <> struct SameSizeInt<f128> : public TB::Type<i128> {};
template <> struct SameSizeInt<i32> : public TB::Type<i32> {};
template <> struct SameSizeInt<i64> : public TB::Type<i64> {};
template <> struct SameSizeInt<i128> : public TB::Type<i128> {};


#define elementsOf(a) (sizeof(a) / sizeof(a[0]))
