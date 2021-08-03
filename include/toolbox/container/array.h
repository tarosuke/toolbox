/** 汎用配列
 * Copyright (C) 2016,2021 tarosuke<webmaster@tarosuke.net>
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

#include <type_traits>

#include <stdlib.h>
#include <string.h>

#include <stdexcept>



namespace TB {
	// is_trivially_constructible

	template <typename T> class Array {
	public:
		Array() : elements(0), assigned(0), body(0){};
		Array(const T origin[], unsigned elements) : elements(0), assigned(0) {
			for (unsigned n(0); n < elements; ++n) {
				Copy(origin[n], n);
			}
		};
		Array(const Array& origin) : elements(0), assigned(0) {
			Resize(0);
			Copy(origin, 0);
		};
		void operator=(const Array& origin) {
			Resize(origin.elements);
			memmove(body, origin.body, sizeof(T) * elements);
		};

		~Array() {
			if (body) {
				Free(TribialConstructable());
			}
		};
		T& operator[](unsigned index) {
			if (elements <= index) {
				throw std::out_of_range("TB:Array");
			}
			return body[index];
		};

		void operator+=(const T& v) { Copy(v, Length()); };
		void operator+=(const Array& v) { Copy(v, Length()); };

		unsigned Size() const { return elements * sizeof(T); };
		unsigned Length() const { return elements; };
		T* Raw() const { return body; };

	protected:
		using TribialConstructable = std::is_trivially_constructible<T>;

		unsigned elements;
		unsigned assigned;
		T* body;

		//サイズ変更
		void Resize(unsigned requierd) {
			if (requierd <= assigned) {
				elements = requierd;
				return;
			}
			const unsigned r(requierd < 16 ? 16 : GetOverPow2(requierd));
			T* newBody(Realloc(r, TribialConstructable()));
			if (!!r && !newBody) {
				throw std::runtime_error("realloc failed in 'TB::Array'");
			}
			body = (T*)newBody;
			assigned = r;
			elements = requierd;
		};
		//最後の要素を削除
		//末尾に追加
		void Copy(const Array& t, unsigned offset) {
			Copy(t, offset, TribialConstructable());
		};
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
		void Copy(const Array& t, unsigned offset, const std::true_type&&) {
			Resize(offset + t.Length());
			memmove(body + offset, t.body, t.Length());
		};
		void Copy(const Array& t, unsigned offset, const std::false_type&&) {
			Resize(offset + t.Length());
			for (unsigned n(0); n < t.elements; ++n) {
				body[n + offset] = t.body[n];
			}
		};
		T* Realloc(unsigned el, const std::true_type&&) {
			return (T*)realloc(body, sizeof(T) * el);
		};
		T* Realloc(unsigned el, const std::false_type&&) {
			T* newBody(new T[el]);

			return newBody;
		};
		void Free(const std::true_type&&) { free(body); };
		void Free(const std::false_type&&) { delete[] body; };
	};
}
