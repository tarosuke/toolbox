/**************************************************************** pthreadなオブジェクト
 */

#include <stdio.h>

#include <toolbox/thread/thread.h>


namespace TB{
	
	Thread::Graveyard Thread::graveyard;
	
	Thread::Thread(bool qwd) : wO::List<Thread>::Node(*this), quitWithDelete(qwd), alive(true){
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		pthread_create(&thread, &attr, Igniter, (void*)this);
	}
	
	Thread::~Thread(){
		pthread_cancel(thread);
		pthread_join(thread, 0);
	}
	
	void* Thread::Igniter(void* p){
		Thread& n(*(Thread*)p);
		n.ThreadBody();
		n.alive = false;
		
		if(n.quitWithDelete){
			graveyard.Bury(n);
		}
		return 0;
	}
	
	
	pthread_t Thread::Graveyard::thread;
	pthread_mutex_t Thread::Graveyard::igniter;
	wO::List<Thread> Thread::Graveyard::bodies;
	
	Thread::Graveyard::Graveyard(){
		//セマフォ初期化
		pthread_mutex_init(&igniter, 0);
		
		//スレッド起動
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		pthread_create(&thread, &attr, Keeper, (void*)this);
	}
	
	void Thread::Graveyard::Bury(wO::List<Thread>::Node& n){
		bodies.Add(n);
		pthread_mutex_unlock(&igniter);
	}
	
	void* Thread::Graveyard::Keeper(void*){
		for(;;){
			//bodyが登録されるまで待つ
			pthread_mutex_lock(&igniter);
			
			//bodyのdelete
			auto t(bodies.Get());
			if(t){
				delete t;
			}
		}
		return 0;
	}
	
}

