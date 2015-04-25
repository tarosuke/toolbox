#pragma once

#include "../thread/thread.h"


namespace wO{

	/** ローカル接続のためのダミーComm兼Commインターフェイス
	 */
	class Comm{
	public:
		virtual void Send(class Message&);
		virtual bool IsServer(){ return false; };
	};

	/** ファイルハンドルベースのComm
	 * Fileと称しているがパイプやソケットのCommなので実質的にはこれがメイン
	 */
	class FileComm : public Comm, public THREAD{
	public:
		FileComm(int file, bool s = false) : handle(file), isServer(s){};
		bool IsServer(){ return isServer; };
	private:
		const int handle; //ファイルハンドル
		const bool isServer;

		void Thread(); //受信処理
	};

	
}
