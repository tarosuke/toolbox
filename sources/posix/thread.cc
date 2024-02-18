/*****************************************************************
 * pthreadなクラス Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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
#include <tb/posix/thread.h>



namespace tb {
	namespace posix {

		Thread::Keeper Thread::keeper;

		Thread::Thread(int sp) : sched_policy(sp) {}
		Thread::~Thread() { pthread_cancel(thread); }
		void* Thread::Entry(void* p) {
			Thread& n(*(Thread*)p);
			n.ThreadBody();
			keeper.Bury(n);
			return 0;
		}
		void Thread::Raise() noexcept(false) {
			// スレッド起動
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setschedpolicy(&attr, sched_policy);
			if (const int r =
					pthread_create(&thread, &attr, Entry, (void*)this))
				throw r;
		}

		void Thread::Cancel() { pthread_cancel(thread); }


		// スレッド起動・破棄スレッド
		pthread_t Thread::Keeper::thread;
		pthread_mutex_t Thread::Keeper::igniter;
		Thread* Thread::Keeper::bodies(0);

		Thread::Keeper::Keeper() {
			// セマフォ初期化
			pthread_mutex_init(&igniter, 0);

			// スレッド起動
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setschedpolicy(&attr, SCHED_IDLE);
			pthread_create(&thread, &attr, Body, 0);
		}


		void Thread::Keeper::Bury(Thread& t) {
			t.next = bodies;
			bodies = t;
			pthread_mutex_unlock(&igniter);
		}

		void* Thread::Keeper::Body(void*) {
			for (;;) {
				// bodyのdelete
				while (Thread* const t = bodies) {
					bodies = (*t).next;
					delete t;
				}

				// bodyが登録されるまで待つ
				pthread_mutex_lock(&igniter);
			}
			return 0;
		}
	}
}
