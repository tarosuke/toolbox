/************************************************************ lock handler:KEY
 *
 *
 */

#pragma once


namespace Lock{

	//RAIIキー
	template<class L> class Key{
	public:
		Key(L& l) : lock(l){ l.Lock(); };
		~Key(){ lock.Unlock(); };
	private:
		L& lock;
	};

	//空ロック
	class NullLock{
		friend class Key<NullLock>;
	private:
		void Lock(){};
		void Unlock(){};
	};

};

