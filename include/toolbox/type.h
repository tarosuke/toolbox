/***** シリアライズ／デシリアライズ付きの型
 */


#pragma once

#include <toolbox/string.h>



namespace TB{

	template <typename T> class Type{
	public:
		Type(const T& t) : body(t){};
		const T& operator=(const T& t){ body = t; return body; };
		operator const T&() const { return body; };

		//serializer and de-serializer
		String Serialize() const;
		const T& operator=(const char*);

	private:
		T body;
	};

}


using u32 = __uint32_t;
using i32 = __int32_t;
using u64 = __uint64_t;
using i64 = __int64_t;
using u128 = __uint128_t;
using i128 = __int128_t;


#define elementsOf(a) (sizeof(a) / sizeof(a[0]))
