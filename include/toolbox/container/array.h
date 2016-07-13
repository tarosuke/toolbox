/************************************************************************ 汎用配列
 * NOTE:コピーコンストラクタや代入演算子はありません。導出して作ってください
 * NOTE:作ったばかりの領域は初期化されていないのでまず書き込むこと
 */
#pragma once

#include <stdlib.h>
#include <assert.h>



namespace TB{

	template<typename T> class Array{
		Array(const Array&);
		void operator=(const Array&);
	public:
		Array(unsigned initialElements=16) :
			elements(initialElements),
			body((T*)malloc(sizeof(T) * initialElements)){
			assert(body);
		};

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
			assert(body);
			if(requierd <= elements){
				return;
			}
			for(;elements < requierd; elements <<= 1);
			body = (T*)realloc(body, sizeof(T) * elements);
			assert(body);
		};
		const T* GetRawBody()const{
			assert(body);
			return body;
		};

	private:
		unsigned elements;
		T* body;
		T dummyEntry;

	};
}
