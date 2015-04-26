
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
		List<Resource>::Node(*this),
		comm(c),
		serverMask(ServerMask()),
		id(NewID()){
		// マップへidとインスタンスのポインタを登録
		id2p[id] = this;
		//Commに登録
		comm.RegisterUser(*this);
	}
	//指定IDで生成
	Resource::Resource(unsigned id, Comm& c) :
		List<Resource>::Node(*this),
		comm(c),
		serverMask(ServerMask()),
		id(id){
		// マップへidとインスタンスのポインタを登録
		id2p[id] = this;
		//Commに登録
		comm.RegisterUser(*this);
	}
	//デストラクタ
	Resource::~Resource(){
		//マップからidとインスタンスのポインタを削除
		id2p.erase(id);
	}

	//サーバマスク計算
	unsigned Resource::ServerMask(){
		return comm.IsServer() ? 1 : 0;
	}

	//新規のIDを取得する
	unsigned Resource::NewID(){
		static unsigned idHead(0);
		int id(0);
		do{
			++idHead;
			id = (idHead << 1) | serverMask;
		}while(id2p.find(id) != id2p.end());
		return id;
	}

	//受信したメッセージを振り分けて蓄積
	void Resource::Received(Message& m){
		const unsigned id(m.GetID());
		if(id2p.find(id) == id2p.end()){
			//未知のIDなので無視
			delete &m;
			return;
		}
		(*id2p[id]).receivedMessages.Add(m);
	}

	//receivedMessagesに入っているメッセージをひとつ取り出してハンドラを呼び出す
	void Resource::GetNextMessage(){
		Message* m(receivedMessages.Get());
		if(m){
			(*m).Deleive(*this);
		}
	}
	void Resource::SendMessage(Message& m){
		comm.Send(m);
	}

}
