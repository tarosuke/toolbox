/*** 自動登録ファクトリ
 * Copyright (C) 2017,2017,2023,2024 tarosuke<webmaster@tarosuke.net>
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
	 * 抽象型をT、派生をUとすると、以下のように使う。
	 * struct T{
	 * 	...
	 *	static tb::Factory* tb::Factory<T>::start; //-- ファクトリのスタック
	 *	...
	 * };
	 * ------
	 * struct U : T{
	 * 	struct F : tb::Factory<TP{
	 *		uint Score() override; //-- 評価のための関数
	 *		T* New() override; //-- Sooreの戻り値が一番大きかったものが呼ばれる
	 *	};
	 *	...
	 *	static F factory; //-- 構築子で自らをスタックに積む
	 * };
	 *
	 * インスタンスを作るときは「tb::Factory<T>::Create();」
	 *
	 * NOTE:Factory<親クラス>::Createを呼ぶのはmainに入った後にすること
	 * NOTE:Score, Newの引数はCreateの引数と同じにすること(異なる方は呼ばれない)
	 */
	template <class T> struct Factory {
		Factory(const Factory&) = delete;
		void operator=(const Factory&) = delete;

		Factory() : next(start) { start = this; };

		/***** Factoryにパラメタを渡すための空の構造体
		 * 実際に何か渡すには継承して、受け取ったらダウンキャストする
		 * NOTE: dynamic_castが0を返すのはassertで確認するレベルの明らかなバグ
		 */
		struct Param {
			virtual ~Param() {};
		};

		static T* Create() {
			Match m;
			for (Factory* f(start); f; f = (*f).next) {
				m.Scored(f, f->Score());
			}
			return m ? m.matched->New() : 0;
		}
		static T* Create(const Param* p) {
			Match m;
			for (Factory* f(start); f; f = (*f).next) {
				m.Scored(f, f->Score(p));
			}
			return m ? m.matched->New(p) : 0;
		}

	protected:
		// Scoreが返す値の目安
		struct Certitude {
			static constexpr uint passiveMatch = 10; // 消極的一致
			static constexpr uint activeMatch = 20; // 種類は一致
			static constexpr uint uniqueMatch = 30; // 個別に一致
		};

		virtual uint Score() { return 0; };
		virtual T* New() { return 0; };
		virtual uint Score(const Param*) { return 0; };
		virtual T* New(const Param*) { return 0; };

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
