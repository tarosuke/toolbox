/** 他プロセス、他環境と共有されるリソースの管理
 */

#pragma once

#include "../container/list.h"


namespace wO{

	class Resource : public List<Resource>::Node{
		Resource();
		Resource(const Resource&);
		void operator=(const Resource&);
	public:
		/** wO::Commが受信したMessageを投入する
		 * Messageの振り分けやdeleteはResourceの仕事
		 */
		static void Received(class Message&);

		/** リソースの通信先を返す
		 */
		class Comm& GetComm() const { return comm; };

	protected:
		/** コンストラクタ＆デストラクタ
		 */
		Resource(class Comm&); //こちらでIDを生成
		Resource(unsigned id, class Comm&); //指定されたIDで
		~Resource();

		/** 受信キューからメッセージをひとつ取り出してハンドラを呼び出す
		 */
		void GetNextMessage();

		/** 設定された送信先へメッセージを送る
		 */
		void SendMessage(class Message&);
	private:
		/** 新しいユニークなIDを生成する
		 * 「ユニーク」とはid→Resource*変換用マップに登録されていないことを意味する
		 */
		unsigned NewID();

		/** commの属性を読んでサーバマスクを計算する
		 */
		unsigned ServerMask();

		/** 使用中のCommがなくなったときに一緒に消滅する
		 */
		void NotifyListDeleted(){ delete this; };

		/** 内部変数
		 */
		class Comm& comm;
		const unsigned serverMask; //IDのマスク
		unsigned id;
		wO::List<Message> receivedMessages;

	};

}
