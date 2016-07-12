/**************************************************************** pthreadなオブジェクト
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
		pthread_t thread;
		static void* Igniter(void*); //スレッドの出入り口

		//墓場
		static class Keeper{
		public:
			Keeper();
			static void Born(Thread&);
			static void Bury(Thread&);
		private:
			static pthread_t thread;
			static pthread_mutex_t igniter; //これが非0になったら処理

			static List<Thread> babies;
			static List<Thread> bodies;
			static void* Body(void*);
		}keeper;
	};

}

