/*** 自動登録ファクトリ
 * Copyright (C) 2017,2017,2023 tarosuke<webmaster@tarosuke.net>
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
 * 		uint Score(...)override;
 * 		U* New(...)override{ return new T; };
 * 	};
 * 	static tb::Factory<U> factory;
 * };
 *
 * またどこかに以下を用意する。
 * tb::Factory<U>* tb::Factory<U>::start(0);
 *
 * Factory<U>::Createを呼ぶとT::Scoreにより適切なT::Newが呼ばれてインsタンスが
 * 戻る
 *
 * NOTE:Factory<親クラス>::Createを呼ぶのはmainに入った後にすること
 */
#pragma once

#include <tb/types.h>



namespace tb {

	template <class T> struct Factory {
		Factory(const Factory&) = delete;
		void operator=(const Factory&) = delete;

		static T* Create() {
			Match m;
			for (Factory* f(start); f; f = (*f).next) {
				m.Scored(f, f->Score());
			}
			return m ? m.matched->New() : 0;
		}

	protected:
		Factory() : next(start) { start = this; };

		virtual uint Score() { return 0; };
		virtual T* New() { return 0; };

	private:
		static Factory* start;
		Factory* const next;

		struct Match {
			uint score;
			Factory* matched;
			operator bool() { return matched && score; };
			Match() : score(0), matched(0){};
			void Scored(Factory* m, uint s) {
				if (score < s) {
					matched = m;
					score = s;
				}
			};
		};
	};
}
