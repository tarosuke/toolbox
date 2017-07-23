/******************************************* timestamp / cyclic sleeper:cyclic
 * 周期処理のためのインフラ(時間はns)
 */
#ifndef _CYCLIC_
#define _CYCLIC_

namespace TB{

	class CYCLIC{
		CYCLIC();
	public:
		CYCLIC(long cycleTime, long preMargin = 0);
		void Sleep();
	private:
		long prevTime; //前回のタイムスタンプ
		long cycleTime; //設定されたサイクルタイム
		long preMargin; //次のサイクルよりちょっと前に起きるための保険
		long GetNanoStamp();
	};

	class DURATION{
	public:
		DURATION();
		double GetDuration();
	private:
		double prevTime;
		double Now();
	};

}

#endif

