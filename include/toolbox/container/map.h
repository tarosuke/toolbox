/** map
 * Copyright (C) 2016 tarosuke<webmaster@tarosuke.net>
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


#include "../lock/key.h"



namespace TB{

	template<
		class T, //管理対象
		typename K, //キー型
		int index = 0, //複数のMapを区別するための番号
		class L=Lock::NullLock> class Map : public L{
		Map(const Map&);
		void operator=(const Map&);
	public:
		//Mapそれ自体のキー
		using Key = Lock::Key<L>;

		/** ノード：Mapのノードはこれをpublicで継承しておく必要がある
		 * 複数のMapを使うときはindexを指定した上で多重継承する
		 *
		 * Nodeがdeleteされると自動的にMapから削除される
		 * Map自体がdeleteされるとNodeは切り離された上でNotifyMapDeletedが呼ばれる
		 * NotifyMapDeletedのデフォルトは「何もしない」なので
		 * Mapが主参照であるならNotifyMapDeletedで自身をdeleteする必要がある
		 */
		class Node{
			friend class Map;
			Node(const Node&);
			void operator=(const Node&);
		public:
			virtual ~Node(){ Detach(); };

		protected:
			Node() : parent(0), nodes(0), left(0), right(0){};
			virtual void NotifyMapDeleted(){};

		private:
			K key;
			Node* parent;
			bool righted; //parentの右に接続されている(parentが0なら無意味)
			unsigned nodes; //以下に接続されているノード数
			Node* left;
			Node* right;

			//左右のバランス調整
			void Balance(){
// 				if(left.nodes + 2 < right.nodes){
// 					//右が重い
// 					Node& nt((*right.link).LeftEdge());
// 					Node& np(left.link ? (*left.link).RightEdge());
//
//
//
//
// 				}else if(right.nodes + 2 < left.nodes){
// 					//左が重い
// 				}
			};
			Node& LeftEdge(){
				return left ? (*left).LeftEdge() : *this;
			};
			Node& RightEdge(){
				return right ? (*right).RightEdge() : *this;
			};




			Node* Find(const K& k){
				if(key == k){
					return this;
				}
				return (*(k < key ? left : right)).Find(k);
			};
			void Add(const K& k, Node& n){
				++nodes;
				auto b(key < k ? right : left);
				if(b){
					(*b).Add(k, n);
				}else{
					b = &n;
					n.parent = this;
				}
			};
			void Detach(){
				//本ノードの「身代わり」を決めて外す
				Node* r;
				Node* p;

				Node** pp(!parent ? 0 : (*parent).left == this ?
					&(*parent).left : &(*parent).right);


				if(left && right){
					//両側あり
					if((*left).nodes < (*right).nodes){
						r = (*right).LeftEdge();
					}else{
						r = (*left).RightEdge();
					}
					(*r).Detach();

					//身代わりをはめ込む
					(*r).left = left;
					(*r).right = right;

					p = (*r).parent;

				}else{
					if(!left){
						r = right;
					}else if(!right){
						r = left;
					}else{
						r = 0;
					}
					p = parent;
				}

				//nodes調整
				for(; p; p = (*p).parent){
					--(*p).nodes;
				}

				//親ノードリンク調整
				if(pp){ *pp = r; }
				if(r){ (*r).parent = parent; }

				//外す
				left = right = parent = 0;
				nodes = 0;
			};

		};



		/** アクセス用クラス
		 * Map::operator[]の戻り値としてのみ存在する
		 * 直接値を取り出すか代入してしか使えないので結果的にMapは配列のように扱える
		 */
		class Ref{
			Ref();
			Ref(const Ref&);
			void operator=(const Ref&);
			Ref* operator*();
			Ref& operator&();
		public:
			operator T*(){ return dynamic_cast<T*>(target); };
			void operator=(T& t){
				//要素の追加あるいは置き換え
				if(head){
					(*head).Add(t);
				}else{
					head = t;
				}
			};
		private:
			Ref(const K& k, T* t) : key(k), target(t){};
			K key;
			Node* const target;
		};


		Map() : head(0){}
		~Map(){
			if(!head){ return; }
			while(Node* n((*head).LeftEdge())){
				(*n).Detach();
				(*n).NotifyMapDeleted();
			}
			while(Node* n((*head).RightEdge())){
				(*n).Detach();
				(*n).NotifyMapDeleted();
			}
			(*head).NotifyMapDeleted();
		};


		/** Mapのアクセスは[]演算子を通して行われる
		 */
		Ref operator[](const K& key){
			Ref ref(key, head ? (*head).Find(key) : 0);
			return ref;
		};

	private:
		Node* head;
	};

}
