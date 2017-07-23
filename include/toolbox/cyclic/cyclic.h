/******************************************* timestamp / cyclic sleeper:cyclic
 * 周期処理のためのインフラ(時間はns)
 */
#ifndef _CYCLIC_
#define _CYCLIC_

namespace TB{

	class CYCLIC{
		CYCLIC();
	public:
		/** CYCLE Key
		 * サイクルを測定するためのクラス
		 * RAIIを使うので図りたい期間Keyのインスタンスを存在させておく
		 */
		class Key{
		public:
			Key(CYCLIC& c) : cyclic(c){ cyclic.Start(); };
			~Key(){ cyclic.End(); }
		private:
			CYCLIC& cyclic;
		};

		CYCLIC(long cycleTime);
		void SleepTo(float ratio); //サイクルタイムのratio分までnanosleepする
	private:
		long prevTime; //前回のタイムスタンプ
		long cycleTime; //設定されたサイクルタイム
		long GetNanoStamp();
		void Start(); //サイクル開始
		void End(); //サイクル終了
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

