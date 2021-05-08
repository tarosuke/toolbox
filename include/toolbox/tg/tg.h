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
#include <toolbox/geometry/matrix.h>



namespace TG {

	//何らかの物体あるいは物体グループ
	class Object : public TB::List<Object>::Node {
		Object(const Object&);
		void operator=(const Object&);

	public:
		virtual ~Object(){};
		virtual void Draw(const TB::Matrix<4, 4>&);

	protected:
		bool visible;
		Object() : visible(true){};
	};

	class Group : public Object {
		Group(const Group&);
		void operator=(const Group&);

	public:
		Group(){};
		void AddChild(Object& o) { children.Add(o); };
		void AddCHild(Group& g) { groups.Add(g); };

	protected:
		TB::List<Object> children;
		TB::List<Object> groups;
		TB::Matrix<4, 4> view;

		void Draw(const TB::Matrix<4, 4>&);
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
		Scene(){};
		virtual ~Scene(){};
		void AddLayer(Object& o) { layers.Add(o); };
		void Draw();

	private:
		TB::List<Object> layers;
	};
}
