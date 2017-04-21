/********************************************************* pthreadなオブジェクト
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
 *
 * スレッドを起こしてそのスレッドでThreadBodyを実行する
 */
#pragma once

#include <pthread.h>

#include <toolbox/container/list.h>


namespace TB{

	class Thread : public List<Thread>::Node{
		Thread(const Thread&);
		void operator=(const Thread&);
	public:
		bool IsThreadAlive(){ return alive; };

	protected:
		Thread(bool qwd = false); //スレッドを起こして起動予約。
		virtual ~Thread();
		virtual void ThreadBody()=0; //本処理:終了するとスレッドは終了する

	private:
		const bool quitWithDelete;
		bool alive;
		Thread* next; //起動、終了スタック用。

		pthread_t thread;
		static void* Entry(void*); //スレッドの出入り口

		//墓場
		static class Keeper{
		public:
			Keeper();
			static void Born(Thread&);
			static void Bury(Thread&);
		private:
			static pthread_t thread;
			static pthread_mutex_t igniter; //これが非0になったら処理

			static Thread* babies;
			static Thread* bodies;
			static void* Body(void*);
		}keeper;
	};

}

