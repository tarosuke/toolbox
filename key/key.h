/************************************************************ lock handler:KEY
 *
 *
 */

#ifndef _KEY_
#define _KEY_



namespace TOOLBOX{

//キー(変数スコープがロックの範囲。uptimeを設定したらIsValidをチェックすること)
template<class LOCK> class KEY{
public:
	KEY(LOCK& lock, long long uptime = -1) :
		valid(lock.Lock(uptime)), lock(lock){};
	~KEY(){ if(valid){ lock.Unlock(); } };
	const bool valid;
private:
	LOCK& lock;
};

//空ロック
class NULLLOCK{
// 	template<class L> friend class KEY;
	friend class KEY<NULLLOCK>;
private:
	bool Lock(long long){ return true; }; //ロックに成功したらture
	void Unlock(){};
};

};

#endif
