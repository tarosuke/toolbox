
#include <map>
#include <assert.h>

#include "resource.h"
#include "message.h"
#include "comm.h"


namespace{
	//idからResourceへの変換マップ：ヘッダにmapをincludeするのを避けるためここに置いてある
	//TODO:stlを使えない環境のために将来的には自前実装に置き換える
	std::map<unsigned, wO::Resource*> id2p;
}

namespace wO{

	//IDを生成して生成
	Resource::Resource(Comm& c) :
		comm(c),
		serverMask(ServerMask()),
		id(NewID()),
		processingMessage(0){
		// マップへidとインスタンスのポインタを登録
		id2p[id] = this;
	}
	//指定IDで生成
	Resource::Resource(unsigned id, Comm& c) :
		comm(c),
		serverMask(ServerMask()),
		id(id),
		processingMessage(0){
		// マップへidとインスタンスのポインタを登録
		id2p[id] = this;
	}
	//デストラクタ
	Resource::~Resource(){
		//マップからidとインスタンスのポインタを削除
		id2p.erase(id);
		//処理中メッセージの解放
		if(processingMessage){
			delete processingMessage;
		}
	}

	//サーバマスク計算
	unsigned Resource::ServerMask(){
		return 0;
//TODO:		return (comm && !(*comm).IsServer()) ? clientServerMask : serverServerMask;
	}

	//新規のIDを取得する
	unsigned Resource::NewID(){
		static unsigned idHead(0);
		do{
			++idHead;
			idHead &= ~serverMask;
			idHead |= serverMask;
		}while(id2p.find(idHead) != id2p.end());
		return idHead;
	}


	//receivedMessagesに入っているメッセージをひとつ取り出して返す
	const Message* Resource::GetNextMessage(){
		if(processingMessage){
			delete processingMessage;
		}
		processingMessage = receivedMessages.Get();
		return processingMessage;
	}
	void Resource::SendMessage(Message& m){
		comm.Send(m);
	}
	


}
