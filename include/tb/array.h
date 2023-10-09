/** 汎用配列
 * Copyright (C) 2016,2021,2023 tarosuke<webmaster@tarosuke.net>
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

#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <tb/key.h>
#include <tb/types.h>
#include <type_traits>



namespace tb {
	template <typename T> struct Array {
		Array() : elements(0), assigned(0), body(0){};
		Array(const T origin[], unsigned elements)
			: elements(0), assigned(0), body(0) {
			for (unsigned n(0); n < elements; ++n) {
				Copy(origin[n], n);
			}
		};
		Array(const Array& origin) : elements(0), assigned(0), body(0) {
			Resize(0);
			Copy(origin, 0);
		};
		void operator=(const Array& origin) {
			Resize(origin.elements);
			memmove(body, origin.body, sizeof(T) * elements);
		};

		~Array() {
			if (body) {
				Free();
			}
		};
		const T& operator[](unsigned index) const {
			if (elements <= index) {
				throw std::out_of_range("TB:Array");
			}
			return body[index];
		};
		T& operator[](unsigned index) {
			if (elements <= index) {
				throw std::out_of_range("TB:Array");
			}
			return body[index];
		};

		void operator+=(const T& v) { Copy(v, Length()); };
		void operator+=(const Array& v) { Copy(v, Length()); };

		bool operator==(const Array& v) const {
			if (elements != v.elements) {
				return false;
			}
			return Compare(v);
		};
		bool operator!=(const Array& t) const { return !(*this == t); };

		unsigned Size() const { return elements * sizeof(T); };
		unsigned Length() const { return elements; };
		T* Raw() const { return body; };

	protected:
		static constexpr bool TribialConstructable =
			std::is_trivially_constructible<T>::value;

		unsigned elements;
		unsigned assigned;
		T* body;

		// サイズ変更
		void Resize(unsigned requierd) {
			if (requierd <= assigned) {
				elements = requierd;
				return;
			}
			const unsigned r(requierd < 16 ? 16 : GetOverPow2(requierd));
			T* newBody(Realloc(r));
			if (!!r && !newBody) {
				throw std::runtime_error("realloc failed in 'TB::Array'");
			}
			body = (T*)newBody;
			assigned = r;
			elements = requierd;
		};
		// 末尾に追加
		void Copy(const T& v, unsigned offset) {
			Resize(offset + 1);
			body[offset] = v;
		};

	private:
		template <typename U> static U GetOverPow2(U n) {
			for (unsigned m(1); m < sizeof(U) * 8; ++m) {
				n |= n >> 1;
			}
			return n + 1;
		};
		bool Compare(const Array& v) const {
			if constexpr (TribialConstructable) {
				return !memcmp(body, v.body, sizeof(T) * elements);
			} else {
				for (unsigned n(0); n < elements; ++n) {
					if (body[n] != v.body[n]) {
						return false;
					}
				}
				return true;
			}
		};
		void Copy(const Array& t, unsigned offset) {
			const unsigned tSize(t.elements);
			Resize(offset + tSize);
			if constexpr (TribialConstructable) {
				memmove(body + offset, t.body, tSize * sizeof(T));
			} else {
				for (unsigned n(0); n < tSize; ++n) {
					body[n + offset] = t.body[n];
				}
			}
		};
		T* Realloc(unsigned el) {
			if constexpr (TribialConstructable) {
				return (T*)realloc(body, sizeof(T) * el);
			} else {
				T* newBody(new T[el]);
				if (newBody) {
					for (unsigned n(0); n < elements; ++n) {
						newBody[n] = body[n];
					}
					delete[] body;
				}
				return newBody;
			}
		};
		void Free() {
			if constexpr (TribialConstructable) {
				free(body);
			} else {
				delete[] body;
			}
		};
	};
}
