#pragma once

#include "vector.h"

namespace wO{

	template<typename T, unsigned dimensions = 2> class Rect{
	public:
		Rect(){};
		Rect(const Vector<T, dimensions>& a, const Vector<T, dimensions>& b) :
			points((const Vector<T, dimensions>[]){ Less(a, b), More(a, b) }){};
		void operator|=(const Rect& t){
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
		Rect operator&(const Rect& t){
			Rect r(*this);
			r &= t;
			return r;
		};
		T operator&&(const Rect& t){
			const wO::Vector<T, dimensions> lt(More(points[0], t.points[0]));
			const wO::Vector<T, dimensions> rb(Less(points[1], t.points[1]));
			T v(0);
			for(unsigned n(0); n < dimensions; ++n){
				const T* const a(lt[n]);
				const T* const b(rb[n]);
				if(b <= a){
					return 0;
				}
				v += b - a;
			}
			return v;
		};
	private:
		Vector<T, dimensions> points[2]; //keep points[0] lesser value
		static Vector<T, dimensions> Less(
			const Vector<T, dimensions>& a, const Vector<T, dimensions>& b){
			T v[dimensions];
			for(unsigned n(0); n < dimensions; ++n){
				const T* const aa(a[n]);
				const T* const bb(b[n]);
				v[n] = aa < bb ? aa : bb;
			}
			return Vector<T, dimensions>(v);
		};
		static Vector<T, dimensions> More(
			const Vector<T, dimensions>& a, const Vector<T, dimensions>& b){
			T v[dimensions];
			for(unsigned n(0); n < dimensions; ++n){
				const T* const aa(a[n]);
				const T* const  bb(b[n]);
				v[n] = aa < bb ? bb : aa;
			}
			return Vector<T, dimensions>(v);
		};
	};

}
