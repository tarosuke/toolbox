
#include <map>

#include "resource.h"
#include "packet.h"


namespace{
	//ヘッダにmapをincludeするのを避けるためここに置いてる。
	std::map<unsigned, wO::Resource*> id2p;
}

namespace wO{

	Resource::Resource() : id(NewID()){
		id2p[id] = this;
	}
	Resource::~Resource(){
		id2p.erase(id);
	}

	//新規のIDを取得する
	unsigned Resource::NewID(){
		static unsigned idHead(0);
		while(idHead++, id2p.find(idHead) != id2p.end());
		return idHead;
	}


	void Resource::Receive(unsigned, Packet* p){ if(p){ delete p; } };
	void Resource::Send(Packet& p){
	}
	


}
