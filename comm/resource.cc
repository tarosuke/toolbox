
#include <map>

#include "resource.h"
#include "message.h"
#include "socket.h"


namespace{
	//idからResourceへの変換マップ：ヘッダにmapをincludeするのを避けるためここに置いてある
	//TODO:stlを使えない環境のために将来的には自前実装に置き換える
	std::map<unsigned, wO::Resource*> id2p;
}

namespace wO{

	Resource::Resource(Socket* s) : socket(s), id(NewID()), processingMessage(0){
		// マップへidとインスタンスのポインタを登録
		id2p[id] = this;
	}
	Resource::~Resource(){
		//マップからidとインスタンスのポインタを削除
		id2p.erase(id);
	}

	//新規のIDを取得する
	unsigned Resource::NewID(){
		static unsigned idHead(0);
		while(++idHead, id2p.find(idHead) != id2p.end());
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
	}
	


}
