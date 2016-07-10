/************************************************************************ 汎用配列
 * NOTE:作ったばかりの領域は初期化されていないのでまず書き込むこと
 */
#pragma once

#include <stdlib.h>


namespace TB{

	template<typename T, unsigned initialSize=16> class Array{

	public:
		Array() :
			limit(initialSize),
			body(malloc(sizeof(T) * initialSize){
		};
		~Array(){
			if(body){
				free(body);
			}
		};
		T& operator[](unsigned index){
			if(limit <= index){
				//伸長処理
				limit *= 2;
				body = realloc(body, sizeof(T) * limit);
			}
			if(!body){
				//確保できなかったなどの理由で無効状態
				return dummyEntry;
			}
			return body[index];
		};
		
	private:
		unsigned limit;
		T* body;
		T dummyEntry;
	};
}
