/** ID管理クラス
 * 1.ID管理
 * 2.IDに結びついたクラスのポインタを管理(格納と取得)
 * ID〜内容のポインタを取り出すときは配列のようにids[id]とする。実装はArrayに。
 */
#pragma once

#include <syslog.h>
#include <stdlib.h>
#include <string.h>



namespace TB{

	template<typename T> class IDArray{
	public:
		class Node{
			Node();
			Node(const Node&);
			void operator=(const Node&);
		public:
			Node(IDArray& arr) : id(GetID(arr)), array(arr){
				//配列の準備
				if(!arr.elements){
					//未確保
					arr.size = 16;
					arr.elements = (Element*)malloc(sizeof(Element) * arr.size);
				}else if(arr.size < id){
					//拡大
					arr.size *= 2;
					arr.elements = (Element*)realloc(
						arr.elements, sizeof(Element*) * arr.size);
				}

				//設定
				arr.elements[id].node = this;
			};
			virtual ~Node(){
				//返却
				array.elements[id].pool = array.pool;
				array.pool = id;
			};
		protected:
			unsigned const id;
		private:
			IDArray& array;
			unsigned GetID(IDArray& arr){
				if(!arr.elements){
 					//初期値は静的に0なので使われていないことを明示的に示しておく
 					arr.pool = ~0U;
				};
				if(~arr.pool){
					//poolに値が入っている
					const unsigned id(arr.pool);
					arr.pool = arr.elements[id].pool;
					return id;
				}
				//再利用できないので新規に割当
				return arr.used++;
			};
		};

		T* operator[](unsigned id){
			return (elements && id < size) ?
				dynamic_cast<T*>(elements[id].node) : 0;
		};
	private:
		union Element{
			Node* node;
			unsigned pool;
		};

		unsigned used;
		unsigned size;
		Element* elements;
		unsigned pool;
	};

}
