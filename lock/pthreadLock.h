

#pragma once

#include <pthread.h>


namespace Lock{

	//pthreadのロック
	class PThreadLock{
		friend class Key<PThreadLock>;
		PThreadLock();
		PThreadLock(const PThreadLock&);
		void operator=(const PThreadLock&);
	public:
		PThreadLock(){
			pthread_mutex_init(&mutex, 0);
		};
		~PTHreadLock(){
			pthread_mutex_destroy(&mutex);
		};
		void Lock(){
			pthread_mutex_lock(&mutex);
		};
		void Unlock(){
			pthread_mutex_unlock(&mutex);
		};
	private:
		pthread_mutex_t mutex;
	};

}
