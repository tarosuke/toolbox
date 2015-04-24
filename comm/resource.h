/** 他プロセス、他環境と共有されるリソースの管理
 */

#pragma once

#include "../container/list.h"


namespace wO{

	class Resource{
		Resource();
		Resource(const Resource&);
		void operator=(const Resource&);
	public:
		/** wO::Socketが受信したMessageを投入する
		 * Messageの振り分けやdeleteはResourceの処理
		 */
		static void Received(class Message&);
		
	protected:
		/** コンストラクタ＆デストラクタ
		 */
		Resource(class Socket*);
		~Resource();

		/** 受信キューからメッセージをひとつ取り出して返す
		 * 返されたメッセージはまたこのメソッドが呼ばれるまで有効
		 */
		const Message* GetNextMessage();
		
		/** 設定された送信先へメッセージを送る
		 */
		void SendMessage(class Message&);
	private:
		/** 新しいユニークなIDを生成する
		 * 「ユニーク」とはid->Resource*変換用マップに登録されていないことを意味する
		 */
		static unsigned NewID();

		class Socket* const socket;
		const unsigned id;
		wO::List<Message> receivedMessages;
		class Message* processingMessage;
	};
	
}
