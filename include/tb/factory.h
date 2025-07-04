/*** 自動登録ファクトリ
 * Copyright (C) 2017,2017,2023,2024,2025 tarosuke<webmaster@tarosuke.net>
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

#include <tb/types.h>



namespace tb {

	/***** 自動登録ファクトリ支援クラス
	 * 抽象型をT、派生をU, スコアリング/生成時のパラメタをARGSとする。
	 * ARGSは可変引数なので0個以上であればよいはずだが、2つ以上は未検証。
	 * struct T{
	 * 	...
	 *	static tb::Factory* tb::Factory<T, ARGS>::start;
	 *	...
	 * };
	 * ------
	 * struct U : T{
	 * 	struct F : tb::Factory<TP{
	 *		uint Score(ARGS) override; //-- 評価のための関数
	 *		T* New(ARGS) override; //-- Sooreの値が一番大きかったものが呼ばれる
	 *	};
	 *	...
	 *	static F factory; //-- 構築子で自らをスタックに積む
	 * };
	 *
	 * インスタンスを作るときは「tb::Factory<T>::Create(ARGS);」
	 *
	 * NOTE:Factory<親クラス>::Createを呼ぶのはmainに入った後にすること
	 * NOTE:Score, Newの引数はCreateの引数と同じにすること
	 */
	template <class T, typename... ARGS> struct Factory {
		Factory(const Factory&) = delete;
		void operator=(const Factory&) = delete;

		Factory() : next(start) { start = this; };

		/***** 最もスコアが高かったクラスをNew
		 */
		static T* Create(ARGS... args) {
			Match m;
			for (Factory* f(start); f; f = (*f).next) {
				m.Scored(f, f->Score(args...));
			}
			return m ? m.matched->New(args...) : 0;
		}

		/***** スコアの評価なしに全部New
		 */
		static void CreateAll(ARGS... args) {
			for (Factory* f(start); f; f = (*f).next) {
				f->New(args...);
			}
		};

	protected:
		// Scoreが返す値の目安
		struct Certitude {
			static constexpr uint passiveMatch = 10; // 消極的一致
			static constexpr uint activeMatch = 20;	 // 種類は一致
			static constexpr uint uniqueMatch = 30;	 // 個別に一致
		};

		virtual uint Score(ARGS...) { return 0; };
		virtual T* New(ARGS...) { return 0; };

	private:
		static Factory* start;
		Factory* const next;

		struct Match {
			uint score;
			Factory* matched;
			operator bool() { return matched && score; };
			Match() : score(0), matched(0) {};
			void Scored(Factory* m, uint s) {
				if (score < s) {
					matched = m;
					score = s;
				}
			};
		};
	};

}
