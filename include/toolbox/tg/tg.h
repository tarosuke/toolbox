/************************************************************ toolbox graphics
 * Copyright (C) 2020, 2021 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/geometry/vector.h>



namespace TG {

	//何らかの物体あるいは物体グループ
	class Object : public TB::List<Object>::Node {
		Object(const Object&);
		void operator=(const Object&);

	public:
		virtual ~Object(){};
	};

	//フレームバッファや画面などの描画先
	class Scene {
	public:
		struct Frustum {
			double left;
			double right;
			double bottom;
			double top;
			double near;
			double far;
		};
		Scene(const Frustum&); // Frustumで初期化
		Scene(const double[]); // 投影行列で初期化
		virtual ~Scene(){};
	};

	class Texture {};

}
