/**************************************************************** pthreadなオブジェクト
 * スレッドを起こしてそのスレッドでThreadBodyを実行する
 */
#pragma once

#include <pthread.h>

#include <toolbox/container/list.h>


namespace TB{
	
	class Thread : public wO::List<Thread>::Node{
		Thread(const Thread&);
		void operator=(const Thread&);
	public:
		virtual ~Thread();
		bool IsThreadAlive(){ return alive; };
		
	protected:
		Thread(bool qwd = false); //スレッドを起こしてスレッドでThreadBodyを実行。
		virtual void ThreadBody()=0; //本処理:終了するとスレッドは終了する
		
	private:
		const bool quitWithDelete;
		bool alive;
		pthread_t thread;
		static void* Igniter(void*); //スレッドの出入り口
		
		//墓場
		static class Graveyard{
		public:
			Graveyard();
			static void Bury(wO::List<Thread>::Node&);
		private:
			static pthread_t thread;
			static pthread_mutex_t igniter; //これが非0になったらdelete処理
			
			static wO::List<Thread> bodies;
			static void* Keeper(void*);
		}graveyard;
	};
	
}

