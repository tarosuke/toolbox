/***** シリアライズ／デシリアライズ付きの型
 */

#include <stdlib.h>
#include <ctype.h>

#include <toolbox/type.h>
#include <toolbox/string.h>



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
			*(__uint32_t*)&body = strtoul(text, 0, 0);
		}else{
			body = strtof(text, 0);
		}
		return body;
	}
	template<> const double& Type<double>::operator=(const char* text){
		if(text[0] == '0' && text[1] == 'x'){
			//ビットイメージで読み込み
			*(__uint64_t*)&body = strtoull(text, 0, 0);
		}else{
			body = strtod(text, 0);
		}
		return body;
	}
	template<> const long double& Type<long double>::operator=(
			const char* text){
		if(text[0] == '0' && text[1] == 'x'){
			//ビットイメージで読み込み
			text += 2;
			__uint128_t v(0);
			for(; *text && isxdigit(*text); ++text){
				v <<= 16;
				v += isdigit(*text) ?
					*text - '0' :
					(*text & ~0x20) - 'A' + 10;
			}
			*(__uint128_t*)&body = v;
		}else{
			body = strtold(text, 0);
		}
		return body;
	}

}
