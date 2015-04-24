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
		/** wO::Commが受信したMessageを投入する
		 * Messageの振り分けやdeleteはResourceの仕事
		 */
		static void Received(class Message&);
		
	protected:
		/** コンストラクタ＆デストラクタ
		 */
		Resource(class Comm*); //こちらでIDを生成 TODO:Comm&を受け取る
		Resource(unsigned id, class Comm*); //指定されたIDで TODO:Comm&を受け取る
		~Resource();

		/** 受信キューからメッセージをひとつ取り出して返す
		 * 返されたメッセージはまたこのメソッドが呼ばれるまで有効
		 * 最後の一個がなかなか解放されないけどキニシナーイ
		 */
		const Message* GetNextMessage();
		
		/** 設定された送信先へメッセージを送る
		 */
		void SendMessage(class Message&);
	private:
		/** 新しいユニークなIDを生成する
		 * 「ユニーク」とはid->Resource*変換用マップに登録されていないことを意味する
		 */
		unsigned NewID();

		/** サーバマスクを計算する
		 */
		unsigned ServerMask();

		/** 内部変数
		 */
		class Comm& comm;
		const unsigned serverMask; //client:0 / server: ~0U >> 1
		unsigned id;
		wO::List<Message> receivedMessages;
		class Message* processingMessage;
	};
	
}
