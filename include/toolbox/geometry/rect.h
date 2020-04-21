/* Rect
 * Copyright (C) 2016 tarosuke<webmaster@tarosuke.net>
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

#include "vector.h"

namespace TB{

	template<typename T, unsigned dimensions = 2> class Rect{
	public:
		Rect(){};
		Rect(const Vector<T, dimensions>& a, const Vector<T, dimensions>& b) :
			points((const Vector<T, dimensions>[]){ Less(a, b), More(a, b) }){};
		void Clear(){
			points[0].Clear();
			points[1].Clear();
		};
		void operator|=(const Rect& t){
			//check volume
			if(!t.HaveVolume()){
				//no volume to append
				return;
			}
			if(!HaveVolume()){
				//copy t
				points[0] = t.points[0];
				points[1] = t.points[1];
				return;
			}
			//append volume
			points[0] =  Less(points[0], t.points[0]);
			points[1] =  More(points[1], t.points[1]);
		};
		Rect operator|(const Rect& t){
			Rect r(*this);
			r |= t;
			return r;
		};
		void operator&=(const Rect& t){
			points[0] =  More(points[0], t.points[0]);
			points[1] =  Less(points[1], t.points[1]);
		};
		Rect operator&(const Rect& t) const{
			Rect r(*this);
			r &= t;
			return r;
		};
		T operator&&(const Rect& t) const{
			const Vector<T, dimensions> lt(More(points[0], t.points[0]));
			const Vector<T, dimensions> rb(Less(points[1], t.points[1]));
			T v(0);
			for(unsigned n(0); n < dimensions; ++n){
				const T a(lt[n]);
				const T b(rb[n]);
				if(b <= a){
					return 0;
				}
				v += b - a;
			}
			return v;
		};
		const T* Left() const { return points[0]; };
		const T* Right() const { return points[1]; };
		Rect operator+(const Vector<T, dimensions>& t) const{
			return Rect(points[0] + t, points[1] + t);
		};
		Rect operator-(const Vector<T, dimensions>& t) const{
			return Rect(points[0] - t, points[1] - t);
		};
		operator bool() const{
			for(unsigned n(0); n < dimensions; ++n){
				if(points[0][n] == points[1][n]){
					return false;
				}
			}
			return true;
		};
		operator T() const{
			T v(1);
			for(unsigned n(0); n < dimensions; ++n){
				v *= points[1][n] - points[0][n];
			}
			return v;
		};
	private:
		Vector<T, dimensions> points[2]; //keep points[0] lesser value
		static Vector<T, dimensions> Less(
			const Vector<T, dimensions>& a, const Vector<T, dimensions>& b){
			T v[dimensions];
			for(unsigned n(0); n < dimensions; ++n){
				const T aa(a[n]);
				const T bb(b[n]);
				v[n] = aa < bb ? aa : bb;
			}
			return Vector<T, dimensions>(v);
		};
		static Vector<T, dimensions> More(
			const Vector<T, dimensions>& a, const Vector<T, dimensions>& b){
			T v[dimensions];
			for(unsigned n(0); n < dimensions; ++n){
				const T aa(a[n]);
				const T bb(b[n]);
				v[n] = aa < bb ? bb : aa;
			}
			return Vector<T, dimensions>(v);
		};
		bool HaveVolume() const{
			for(unsigned n(0); n < dimensions; ++n){
				if(points[1][n] <= points[0][n]){
					//no volume
					return false;
				}
			}
			return true;
		};
	};

}
