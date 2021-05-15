/***************************************************************** pthreadなクラス
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>

#include <toolbox/thread/pthread.h>


namespace TB{

	PThread::Keeper PThread::keeper;

	PThread::PThread(bool qwd, int sp) :
		quitWithDelete(qwd),
		alive(true),
		sched_policy(sp){}
	PThread::~PThread(){
		pthread_cancel(thread);
	}
	void* PThread::Entry(void* p){
		PThread& n(*(PThread*)p);
		n.ThreadBody();
		n.alive = false;

		if(n.quitWithDelete){
			keeper.Bury(n);
		}
		pthread_cancel(n.thread);
		return 0;
	}
	void PThread::RaiseThread(){
		//スレッド起動
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, sched_policy);
		pthread_create(
			&thread,
			&attr,
			Entry,
			(void*)this);
	}


	//スレッド起動・破棄スレッド

	pthread_t PThread::Keeper::thread;
	pthread_mutex_t PThread::Keeper::igniter;
	TB::PThread* PThread::Keeper::bodies(0);

	PThread::Keeper::Keeper(){
		//セマフォ初期化
		pthread_mutex_init(&igniter, 0);

		//スレッド起動
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setschedpolicy(&attr, SCHED_IDLE);
		pthread_create(&thread, &attr, Body, 0);
	}


	void PThread::Keeper::Bury(PThread& t){
		t.next = bodies;
		bodies = t;
		pthread_mutex_unlock(&igniter);
	}

	void* PThread::Keeper::Body(void*){
		for(;;){
			//bodyのdelete
			while(PThread* const t = bodies){
				bodies = (*t).next;
				delete t;
			}

			//bodyが登録されるまで待つ
			pthread_mutex_lock(&igniter);
		}
		return 0;
	}

}
