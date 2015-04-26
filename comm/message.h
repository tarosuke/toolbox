#pragma once

#include "../container/list.h"


namespace wO{

	class Message : public List<Message>::Node{
		Message();
	public:
		enum{
			helo,
			disconnect,
		};
		static const unsigned maxLen = 512;
		struct Head{
			unsigned short len; //lenはmaxLen以下なのでMSBは0
			unsigned short type; //typeのMSBは1
			unsigned id;
			unsigned char body[];
		};
		static Message& New(int, void (*)(int, void*, unsigned));
		virtual ~Message();
		virtual void Deleive(class Resource&); //指定Resourceの所定ハンドラへMessage転送
		unsigned short GetType(){ return body.head.type; };
		unsigned GetID(){ return body.head.id; };
	protected:
		Message(Head&, bool) ;
		virtual void Localize(){}; //エンディアン変換
		const bool reverseEndian;
		union{
			Head head;
			unsigned char raw[maxLen];
		}body;
	private:
		void NotifyListDeleted(){ delete this; }; //つながってるList自体がなくなった時は消滅
	};

}
