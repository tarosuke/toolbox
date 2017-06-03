/** ID管理クラス
 * 1.ID管理
 * 2.IDに結びついたクラスのポインタを管理(格納と取得)
 * ID〜内容のポインタを取り出すときは配列のようにids[id]とする。実装はArrayに。
 */
#pragma once

#include <syslog.h>

#include <toolbox/container/array.h>



namespace TB{


	template<typename T, unsigned offset = 0>class IDArray{
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
				pool = array[id].next;
syslog(LOG_INFO, "TB::recycle ID:%u", id);
			}else{
				//新規割当
				id = used++;
syslog(LOG_INFO, "TB::new ID:%u", id);
			}

			//値を格納して終了
			array[id] = (E){ content, 0 };
			return id + offset;
		};

		//IDを返却する
		void ReleaseID(unsigned id){
			id -= offset;
			E& e(array[id]);
			e.content = 0;
			e.next = pool;
			pool = id;
		};


		//値の設定
		void SetContent(unsigned id, T* content){
			array[id - offset] = (E){ content, 0 };
		};

		//値の取得
		T* operator[](unsigned id){
			id -= offset;
			if(used <= id){ return 0; }
			E& e(array[id]);
			return e.next ? 0 : e.content;
		};

	private:
		//管理領域(unionだと使用中判定しにくいのでstructにした)
		struct E{
			T* content;
			unsigned next;
		};
		unsigned used; //すでに払いだされた値
		unsigned pool; //返却されたIDのスタック
		Array<E> array;
	};


}
