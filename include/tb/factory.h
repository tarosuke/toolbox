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
 *
 *
 * 下のようにして使う。
 * struct T : U{
 * 	...
 * 	struct V : tb::Factory<U>{
 * 		uint Score()override;
 * 		U* New()override{ return new T; };
 * 	};
 * 	static tb::Factory<U> factory;
 * };
 *
 * またどこかに以下を用意する。Uと同じソースがいいだろう。
 * tb::Factory<U>* tb::Factory<U>::start(0);
 *
 * さらに各クラス(T)のファクトリ(T::V)のインスタンスを関数外に用意する。
 *
 * Factory<U>::Createを呼ぶとT::Scoreにより適切なT::Newが選択されて呼ばれる。
 * なおScoreの戻り値が最大のものが選択される。同値の場合は先に呼ばれた方になる
 * のでどれが選択されるかはリンク順次第になる。
 *
 * NOTE:Factory<親クラス>::Createを呼ぶのはmainに入った後にすること
 * NOTE:Score, Newの引数はCreateの引数と同じにすること(異なる方は呼ばれない)
 */
#pragma once

#include <tb/types.h>



namespace tb {

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
		enum class Certitude : uint {
			passiveMatch = 10, // 消極的一致
			activeMatch = 20, // 種類は一致
			uniqueMatch = 30, // 個別に一致
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
