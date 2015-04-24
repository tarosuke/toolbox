#pragma once

#include "../container/list.h"

namespace wO{

	class Message : public List<Message>::Node{
	private:
		unsigned id;
	};

}
