/************************************************************************ 汎用配列
 * NOTE:コピーコンストラクタや代入演算子はありません。導出して作ってください
 * NOTE:作ったばかりの領域は初期化されていないのでまず書き込むこと
 */
#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>



namespace TB{

	template<typename T> class Array{
		Array(const Array&);
		void operator=(const Array&);
	public:
		Array() : elements(0), body(0){};

		~Array(){
			if(body){ free(body); }
		};
		T& operator[](unsigned index){
			Resize(index + 1);
			if(!body){
				//確保できなかったなどの理由で無効状態
				return dummyEntry;
			}
			return body[index];
		};

	protected:
		void Resize(unsigned requierd){
			if(requierd <= elements){
				return;
			}
			const unsigned oldElements(elements);
			if(!elements){ elements = 16; }
			for(;elements < requierd; elements <<= 1);
			body = (T*)realloc(body, sizeof(T) * elements);
			assert(body); //TODO:継続動作するための方法を考えておく
			memset(&body[oldElements], 0, elements - oldElements);
		};
		T* GetRawBody()const{
			assert(body);
			return body;
		};
		void Set(unsigned index, T content){
			Resize(index + 1);
			body[index] = content;
		};
		T Get(unsigned index){
			Resize(index + 1);
			return body[index];
		};

	private:
		unsigned elements;
		T* body;
		T dummyEntry;

	};
}
