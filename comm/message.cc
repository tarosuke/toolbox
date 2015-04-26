#include <stdlib.h>
# include <bits/byteswap.h>

#include "message.h"


namespace wO{

	Message& Message::New(int handle, void (*reader)(int, void*, unsigned)){
		//ヘッダ待ち
		Head h;
		reader(handle, &h, sizeof(h));

		//エンディアン検出
		const bool reverseEndian(!(h.type & 0x8000));

		//ヘッダのエンディアン変換
		if(reverseEndian){
			h.len = __bswap_16(h.len);
			h.type = __bswap_16(h.type);
			h.id = __bswap_32(h.id);
		}

		//タイプ別インスタンス確保
		Message* m(0);
		switch(h.type){
		default:
			m = new Message(h, reverseEndian);
			break;
		}
		if(!m){
			throw -1;
		}

		//コンテント読み込み
		reader(handle, (*m).body.head.body, h.len);

		//エンディアン変換
		(*m).Localize();

		return *m;
	}

	Message::Message(Head& head, bool re) :
		List<Message>::Node(*this), reverseEndian(re){
		body.head = head;
	};
	Message::~Message(){
	}

	//転送(ただしこいつは未定義メッセージなので単に破棄)
	void Message::Deleive(Resource&){
		delete this;
	}

}
