/***** シリアライズ／デシリアライズ付きの型
 */

#include <stdlib.h>
#include <ctype.h>

#include <toolbox/type.h>



namespace{

	const char* const num = "0123456789abcdef";

	char* UDec(u128 v, char b[64]){
		char* p(&b[63]);
		*p-- = 0;
		do{
			*p-- = num[v % 10];
			v /= 10;
		}while(v);
		return p + 1;
	}
	char* Dec(i128 v, char b[64]){
		char* p(UDec((u128)v, b));
		if(0 <= v){
			return p;
		}
		*--p = '-';
		return p;
	}
	// char* Hex(u128 v, char b[64]){
	// 	char* p(&b[63]);
	// 	*p-- = 0;
	// 	do{
	// 		*p-- = num[v & 0xf];
	// 		v >>= 4;
	// 	}while(v);
	// 	*p = 'x';
	// 	*--p = '0';
	// 	return p;
	// }
}

namespace TB{

	template<> const String& Type<String>::operator=(const char* text){
		body = text;
		return body;
	}

	template<> const int& Type<int>::operator=(const char* text){
		body = atoi(text);
		return body;
	}
	template<> const long& Type<long>::operator=(const char* text){
		body = strtol(text, 0, 0);
		return body;
	}
	template<> const long long& Type<long long>::operator=(
			const char* text){
		body = strtoll(text, 0, 0);
		return body;
	}
	template<> const unsigned& Type<unsigned>::operator=(const char* text){
		body = strtoul(text, 0, 0);
		return body;
	}
	template<> const unsigned long& Type<unsigned long>::operator=(
			const char* text){
		body = strtoul(text, 0, 0);
		return body;
	}
	template<> const unsigned long long& Type<unsigned long long>::operator=(
			const char* text){
		body = strtoull(text, 0, 0);
		return body;
	}

	template<> const float& Type<float>::operator=(const char* text){
		if(text[0] == '0' && text[1] == 'x'){
			//ビットイメージで読み込み
			union {
				u32 raw;
				float val;
			} u;
			u.raw = strtoul(text, 0, 0);
			body = u.val;
		} else {
			body = strtof(text, 0);
		}
		return body;
	}
	template<> const double& Type<double>::operator=(const char* text){
		if(text[0] == '0' && text[1] == 'x'){
			//ビットイメージで読み込み
			union {
				u64 raw;
				float val;
			} u;
			u.raw = strtoull(text, 0, 0);
			body = u.val;
		} else {
			body = strtod(text, 0);
		}
		return body;
	}
	template<> const long double& Type<long double>::operator=(
			const char* text){
		if(text[0] == '0' && text[1] == 'x'){
			//ビットイメージで読み込み
			text += 2;
			u128 v(0);
			for(; *text && isxdigit(*text); ++text){
				v <<= 16;
				v += isdigit(*text) ?
					*text - '0' :
					(*text & ~0x20) - 'A' + 10;
			}

			union {
				u128 raw;
				long double val;
			} u;
			u.raw = v;
			body = u.val;
		}else{
			body = strtold(text, 0);
		}
		return body;
	}


	template<> String Type<String>::Serialize() const {
		return body;
	}
	template<> String Type<int>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template<> String Type<uint>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	template<> String Type<long>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template<> String Type<unsigned long>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	template<> String Type<long long>::Serialize() const {
		char b[64];
		return String(Dec(body, b));
	}
	template<> String Type<unsigned long long>::Serialize() const {
		char b[64];
		return String(UDec(body, b));
	}
	// template<> String Type<float>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u32*)&body, b));
	// }
	// template<> String Type<double>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u64*)&body, b));
	// }
	// template<> String Type<long double>::Serialize() const {
	// 	char b[64];
	// 	return String(Hex(*(u128*)&body, b));
	// }
}
