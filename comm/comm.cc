
#include "comm.h"
#include "message.h"


namespace wO{

	/** FileComm
	 */
	void Comm::Send(Message& m){
		delete &m;
	}

	//受信処理。handle自体にエラーを検出したら終了(=消滅)
	void FileComm::Thread(){
	}

}
