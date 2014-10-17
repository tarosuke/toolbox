/************************************************************ lock handler:KEY
 *
 *
 */

#pragma once


namespace Lock{

	//RAIIキー
	template<class L> class Key{
	public:
		KEY(L& l) : lock(l){ l.lock(); };
		~KEY(){ lock.Unlock(); };
	private:
		L& lock;
	};

	//空ロック
	class NullLock{
		friend class KEY<NullLock>;
	private:
		void Lock(){};
		void Unlock(){};
	};

};

#endif
