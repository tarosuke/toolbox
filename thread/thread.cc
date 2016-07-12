/**************************************************************** pthreadなオブジェクト
 */

#include <stdio.h>

#include <toolbox/thread/thread.h>


namespace TB{

	Thread::Keeper Thread::keeper;

	Thread::Thread(bool qwd) :
		TB::List<Thread>::Node(*this),
		quitWithDelete(qwd),
		alive(true){
		Keeper::Born(*this);
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
			keeper.Bury(n);
		}
		return 0;
	}



//スレッド起動・破棄スレッド

	pthread_t Thread::Keeper::thread;
	pthread_mutex_t Thread::Keeper::igniter;
	TB::List<Thread> Thread::Keeper::babies;
	TB::List<Thread> Thread::Keeper::bodies;

	Thread::Keeper::Keeper(){
		//セマフォ初期化
		pthread_mutex_init(&igniter, 0);

		//スレッド起動
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		pthread_create(&thread, &attr, Body, 0);
	}


	void Thread::Keeper::Born(Thread& t){
		bodies.Add(t);
		pthread_mutex_unlock(&igniter);
	}
	void Thread::Keeper::Bury(Thread& t){
		bodies.Add(t);
		pthread_mutex_unlock(&igniter);
	}

	void* Thread::Keeper::Body(void*){
		for(;;){
			//babyかbodyが登録されるまで待つ
			pthread_mutex_lock(&igniter);

			//babyの起動
			while(auto t = babies.Get()){
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
				pthread_create(
					&(*t).thread,
					&attr,
					(*t).Igniter,
					(void*)t);
			}

			//bodyのdelete
			while(auto t = bodies.Get()){
				delete t;
			}
		}
		return 0;
	}

}

