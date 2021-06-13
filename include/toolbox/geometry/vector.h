/** Vector
 * Copyright (C) 2016, 2017, 2021 tarosuke<webmaster@tarosuke.net>
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

#include <math.h>



namespace TB{

	template <unsigned D, typename T = float> class Vector {
	public:
		operator T*(){
			return value;
		};
		operator const T*() const{
			return value;
		};
		Vector(T x, T y){ value[0] = x; value[1] = y; };
		Vector(T x, T y, T z){ value[0] = x; value[1] = y; value[2] = z; };
		Vector& operator=(const Vector& t){
			for (unsigned n(0); n < D; ++n) {
				value[n] = t[n];
			}
			return *this;
		};
		template<typename U> void operator=(const U t[]){
			for (unsigned n(0); n < D; ++n) {
				value[n] = (T)t[n];
			}
		};
		template <typename U> Vector(const Vector<D, U>& iv) {
			for (unsigned n(0); n < D; ++n) {
				value[n] = (T)iv[n];
			}
		};
		Vector(){ Clear(); };
		void Clear(){
			for (unsigned n(0); n < D; ++n) {
				value[n] = 0;
			}
		};
		template<typename U> Vector(const U iv[]){
			*this = iv;
		};
		bool operator==(const Vector& t) const{
			for (unsigned n(0); n < D; ++n) {
				if(value[n] != t.value[n]){
					return false;
				}
			}
			return true;
		};
		bool operator!=(const Vector& t) const{
			return !operator==(t);
		};
		Vector operator+(const Vector& t) const{
			Vector r;
			for (unsigned n(0); n < D; ++n) {
				r.value[n] = value[n] + t.value[n];
			}
			return r;
		};
		void operator+=(const Vector& t){
			for (unsigned n(0); n < D; ++n) {
				value[n] += t.value[n];
			}
		};
		Vector operator-(const Vector& t) const{
			Vector r;
			for (unsigned n(0); n < D; ++n) {
				r.value[n] = value[n] - t.value[n];
			}
			return r;
		};
		void operator-=(const Vector& t){
			for (unsigned n(0); n < D; ++n) {
				value[n] -= t.value[n];
			}
		};
		void operator*=(T t){
			for (unsigned n(0); n < D; ++n) {
				value[n] *= t;
			}
		};
		Vector operator*(T t){
			Vector v(*this);
			v *= t;
			return v;
		};
		void operator/=(T t){
			for (unsigned n(0); n < D; ++n) {
				value[n] /= t;
			}
		};
		Vector operator/(T t) {
			Vector v(*this);
			v /= t;
			return v;
		};
		T operator*(const Vector& t) const {
			T v(0);
			for (unsigned n(0); n < D; ++n) {
				v += value[n] * t.value[n];
			}
			return v;
		};
		void Min(const Vector& t) {
			for (unsigned n(0); n < D; ++n) {
				if(t.value[n] < value[n]){
					value[n] = t.value[n];
				}
			}
		};
		void Max(const Vector& t){
			for (unsigned n(0); n < D; ++n) {
				if(value[n] < t.value[n]){
					value[n] = t.value[n];
				}
			}
		};

		T Length2() const {
			T l2(0);
			for (const T e : value) {
				l2 += e;
			}
			return l2;
		};
		T Length() const { return sqrt(Length2()); };

		void Normalize(){
			*this /= Length();
		};

		T Cross2(const Vector& t) const {
			return value[0] * t.value[1] - value[1] * t.value[0];
		};
		Vector Cross(const Vector&) const;

	private:
		T value[D];
	};
};
