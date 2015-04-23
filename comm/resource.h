#pragma once

namespace wO{

	class Resource{
		Resource(const Resource&);
		void operator=(const Resource&);
	protected:
		Resource();
		virtual ~Resource();
		virtual void Receive(unsigned, class Packet*);
		void Send(class Packet&);
	private:
		static unsigned NewID();
		const unsigned id;
	};
	
}
