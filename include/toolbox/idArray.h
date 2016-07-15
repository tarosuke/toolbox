/** ID管理クラス
 * 1.ID管理
 * 2.IDに結びついたクラスのポインタを管理(格納と取得)
 * ID〜内容のポインタを取り出すときは配列のようにids[id]とする。実装はArrayに。
 */
#pragma once

#include <toolbox/container/array.h>


namespace TB{


	template<typename T> class IDArray : Array<T*>{
		IDArray(const IDArray&);
		void operator=(const IDArray&);
	public:
		IDArray() : used(0), pool(0){};

		//IDを新規に確保して引数のT*の値を格納する
		unsigned GetID(T* content){
			unsigned id;
			if(pool){
				//スタックから再利用
				id = pool;
				pool = static_cast<unsigned>((*this)[id]);
			}else{
				//新規割当
				id = used++;
			}

			//値を格納して終了
			(*this)[id] = content;
			return id;
		};

		//IDを返却する
		void ReleaseID(unsigned id){
			(*this)[id] = static_cast<T*>(pool);
			pool = id;
		};

		//値の取得
		T* operator[](unsigned id){
			if(used <= id){ return 0; }
			//TODO:(*this)(id)の値をチェックして返却済IDなら0を返す
			return (*this)[id};
		};

	private:
		unsigned used; //すでに払いだされた値
		unsigned pool; //返却されたIDのスタック
	};


}
