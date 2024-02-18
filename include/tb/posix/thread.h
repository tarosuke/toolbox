/********************************************************* pthreadなオブジェクト
 * Copyright (C) 2017, 2024 tarosuke<webmaster@tarosuke.net>
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
 *
 * スレッドを起こしてそのスレッドでThreadBodyを実行する
 */
#pragma once

#include <pthread.h>

#include <tb/list.h>



namespace tb {

	namespace posix {

		struct Thread : public tb::List<Thread>::Node {
			Thread(const Thread&) = delete;
			void operator=(const Thread&) = delete;

		protected:
			/** スレッド生成
			 * NOTE:スレッドが起動するのはRaiseを呼んだ時
			 * schedPolicyの詳細はsched(7)を参照。
			 */
			Thread(int schedPolicy = SCHED_OTHER);
			virtual ~Thread();
			virtual void
			ThreadBody() = 0; // 本処理:終了するとスレッドは終了する
			void Raise() noexcept(false);
			void Cancel();

		private:
			Thread* next = 0; // 終了スタック用。

			pthread_t thread;
			int sched_policy;
			static void* Entry(void*); // スレッドの出入り口

			// 墓場
			static class Keeper {
			public:
				Keeper();
				static void Bury(Thread&);

			private:
				static pthread_t thread;
				static pthread_mutex_t igniter; // これが非0になったら処理
				static Thread* bodies;
				static void* Body(void*);
			} keeper;
		};
	}
}
