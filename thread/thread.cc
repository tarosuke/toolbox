/***************************************************************** pthreadなクラス
 */

#include <stdio.h>

#include <toolbox/thread/thread.h>


namespace TB{

	Thread::Keeper Thread::keeper;

	Thread::Thread(bool qwd) :
		quitWithDelete(qwd),
		alive(true){
		Keeper::Born(*this);
	}
	Thread::~Thread(){
		pthread_cancel(thread);
		pthread_join(thread, 0);
	}
	void* Thread::Entry(void* p){
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
	TB::Thread* Thread::Keeper::babies(0);
	TB::Thread* Thread::Keeper::bodies(0);

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
		t.next = babies;
		babies = t;
		pthread_mutex_unlock(&igniter);
	}
	void Thread::Keeper::Bury(Thread& t){
		t.next = bodies;
		bodies = t;
		pthread_mutex_unlock(&igniter);
	}

	void* Thread::Keeper::Body(void*){
		for(;;){
			//babyの起動
			while(Thread* const t = babies){
				//babiesから要素をひとつ取り出す
				babies = (*t).next;

				//スレッド起動
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
				pthread_create(
					&(*t).thread,
					&attr,
					(*t).Entry,
					(void*)t);
			}

			//bodyのdelete
			while(Thread* const t = bodies){
				bodies = (*t).next;


				delete t;
			}

			//babyかbodyが登録されるまで待つ
			pthread_mutex_lock(&igniter);
		}
		return 0;
	}

}

