/******************************************* timestamp / cyclic sleeper:cyclic
 * 周期処理のためのインフラ(時間はns)
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
 */
#pragma once

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
