#pragma once

#include "../container/list.h"

namespace wO{

	class Message : public List<Message>::Node{
	public:
	private:
		void NotifyListDeleted(){ delete this; }; //つながってるList自体がなくなった時は消滅
		unsigned id;
	};

}
