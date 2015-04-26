#pragma once

#include "../thread/thread.h"
#include "../container/list.h"


namespace wO{

	/** ローカル接続のためのダミーComm兼Commインターフェイス
	 */
	class Comm{
	public:
		virtual void Send(class Message&);
		virtual bool IsServer(){ return false; };
		void RegisterUser(class Resource&);
	private:
		List<class Resource> users;
	};

	/** ファイルハンドルベースのComm
	 * Fileと称しているがパイプやソケットのCommなので実質的にはこれが本体
	 */
	class FileComm : public Comm, public THREAD{
	public:
		FileComm(int file, bool s = false) : handle(file), isServer(s){};
		bool IsServer(){ return isServer; };
	private:
		const int handle; //ファイルハンドル
		const bool isServer; //サーバならtrue

		void Thread(); //受信処理
		static void Read(int, void*, unsigned);
	};

}
