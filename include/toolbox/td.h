/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/container/list.h>
#include <toolbox/geometry/matrix.h>
#include <toolbox/geometry/spread.h>



namespace TB {

	struct TD {
		using M44 = Matrix<4, 4, float>;
		using S2 = Spread<2, unsigned>;

		static M44 Frustum();


		struct Object : public List<Object>::Node {
			virtual void Draw(/*未定*/) = 0;
		};



		void AddHead(Object& o) { head.Add(o); };
		void AddExternal(Object& o) { external.Add(o); };
		void AddScenery(Object& o) { scenery.Add(o); };


		virtual void Draw(const M44& view) = 0;

		virtual ~TD(){};

	protected:
		TD(){};


	private:
		struct Target {
			Target() : regenerate(false){};
			void Add(Object& o) { list.Add(o); };
			List<Object> list;
			bool regenerate; //コマンド再生成が必要
		} head, external, scenery;
	};
}
