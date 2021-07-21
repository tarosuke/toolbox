/******************************************************************** 汎用配列
 */
#pragma once

#include <stdlib.h>
#include <string.h>

#include <stdexcept>



namespace TB{

	template<typename T> class Array{
		Array(const Array&);
		void operator=(const Array&);
	public:
		Array() : elements(0), assigned(0), body(0){};
		Array(const T* origin, unsigned elements)
			: elements(elements), assigned(elements),
			  body(malloc(sizeof(T) * elements)){};

		~Array(){
			if(body){ free(body); }
		};
		T& operator[](unsigned index){
			if (elements <= index) {
				throw std::out_of_range("TB:Array");
			}
			return body[index];
		};

		unsigned Size() { return elements * sizeof(T); };
		unsigned Length() { return elements; };

	protected:
		void Resize(unsigned requierd) {
			if (requierd <= elements) {
				return;
			}
			if (requierd <= assigned) {
				elements = requierd;
				return;
			}
			const unsigned r(GetOverPow2(requierd));
			void* const newBody(realloc(body, sizeof(T) * r));
			if (!newBody) {
				throw std::runtime_error("realloc failed in 'TB::Array'");
			}
			body = (T*)newBody;
		};
		T* GetRawBody() const { return body; };
		void PushBack(const T& v) {
			Resize(elements + 1);
			body[elements] = v;
		};

	private:
		unsigned elements;
		unsigned assigned;
		T* body;

		template <typename U> static U GetOverPow2(U n) {
			n -= 1;
			for (unsigned m(1); m < sizeof(U) * 8; m <<= 1) {
				n |= n >> m;
			}
			return n + 1;
		};
	};
}
