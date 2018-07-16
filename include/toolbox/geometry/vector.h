/** Vector
 * Copyright (C) 2016, 2017 tarosuke<webmaster@tarosuke.net>
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

	template<typename T, unsigned dimension = 2> class Vector{
	public:
		operator T*(){
			return value;
		};
		operator const T*() const{
			return value;
		};
		Vector(T x, T y){ value[0] = x; value[1] = y; };
		Vector(T x, T y, T z){ value[0] = x; value[1] = y; value[2] = z; };
		template<typename U> void operator=(const U t[]){
			for(unsigned n(0); n < dimension; ++n){
				value[n] = (T)t[n];
			}
		};
		template<typename U> Vector(const Vector<U, dimension>& iv){
			for(unsigned n(0); n < dimension; ++n){
				value[n] = (T)iv[n];
			}
		};
		Vector(){ Clear(); };
		void Clear(){
			for(unsigned n(0); n < dimension; ++n){
				value[n] = 0;
			}
		};
		template<typename U> Vector(const U iv[]){
			*this = iv;
		};
		bool operator==(const Vector& t) const{
			for(unsigned n(0); n < dimension; ++n){
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
			for(unsigned n(0); n < dimension; ++n){
				r.value[n] = value[n] + t.value[n];
			}
			return r;
		};
		void operator+=(const Vector& t){
			for(unsigned n(0); n < dimension; ++n){
				value[n] += t.value[n];
			}
		};
		Vector operator-(const Vector& t) const{
			Vector r;
			for(unsigned n(0); n < dimension; ++n){
				r.value[n] = value[n] - t.value[n];
			}
			return r;
		};
		void operator-=(const Vector& t){
			for(unsigned n(0); n < dimension; ++n){
				value[n] -= t.value[n];
			}
		};
		void operator*=(T t){
			for(unsigned n(0); n < dimension; ++n){
				value[n] *= t;
			}
		};
		void operator/=(T t){
			for(unsigned n(0); n < dimension; ++n){
				value[n] /= t;
			}
		};
		void Min(const Vector& t){
			for(unsigned n(0); n < dimension; ++n){
				if(t.value[n] < value[n]){
					value[n] = t.value[n];
				}
			}
		};
		void Max(const Vector& t){
			for(unsigned n(0); n < dimension; ++n){
				if(value[n] < t.value[n]){
					value[n] = t.value[n];
				}
			}
		};

		T X() const { return value[0]; };
		T Y() const { return value[1]; };
		T Z() const { return value[2]; };

		T Length() const {
			T l2(0);
			for(unsigned n(0); n < dimension; ++n){
				l2 += value[n] * value[n];
			}
			return sqrt(l2);
		};

		void Normalize(){
			*this /= Length();
		};

	private:

		T value[dimension];
	};


};
