#pragma once


namespace wO{

	template<typename T, unsigned dimension = 2> class Vector{
	public:
		operator const T*(){
			return value;
		};
		template<typename U> void operator=(const U t[]){
			for(unsigned n(0); n < dimension; ++n){
				value[n] = (T)t[n];
			}
		};
		template<typename U> Vector(const Vector<U, dimension>& iv){
			*this = iv;
		};
		Vector(){};
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
		Vector operator-(const Vector& t) const{
			Vector r;
			for(unsigned n(0); n < dimension; ++n){
				r.value[n] = value[n] - t.value[n];
			}
			return r;
		};
	private:
		T value[dimension];
	};


};

