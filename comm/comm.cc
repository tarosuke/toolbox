
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "comm.h"
#include "message.h"
#include "resource.h"


namespace wO{

	//ユーザー登録
	void Comm::RegisterUser(Resource& r){
		users.Add(r);
	}

	/** FileComm
	 */
	void Comm::Send(Message& m){
		delete &m;
	}

	//受信処理。handle自体にエラーを検出したら終了(=消滅)
	void FileComm::Thread(){
		try{
			for(;;){
				Message& m(Message::New(handle, Read));

				switch(m.GetType()){
				case Message::helo : //接続時メッセージ
					//TODO:認証とかするところ
					delete &m;
					break;
				case Message::disconnect : //切断リクエスト
					delete &m;
					throw -1;
					break;
				default:
					//Resourceへ投入
					Resource::Received(m);
					break;
				}
			}
		}
		catch(...){
			close(handle);
		}
		/** このメソッドを抜けるとこのインスタンスは「墓場」送りになる
		 * だが消滅と同時にこれを使ってるResourceも消滅するようになっているのでキニスンナ
		 */
	}

	//指定サイズを読み込む
	void FileComm::Read(int handle, void* to, unsigned bytes){
		const int status(read(handle, to, bytes));
		if((int)bytes != status){
			throw status;
		}
	}

}
