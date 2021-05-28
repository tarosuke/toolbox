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

	class Object;
	class Scenery;

	//フレームバッファや画面などの描画先
	class Scene {
		Scene(const Scene&);
		void operator=(const Scene&);

	public:
		struct Frustum {
			double left;
			double right;
			double bottom;
			double top;
			double near;
			double far;
		};

		Scene();
		virtual ~Scene();

		void SetFrustum(const Frustum& frustum);
		void SetProjectionMatrix(const double projectionMatrix[]);

		//カメラ操作
		void SetView(const TB::Matrix<4, 4, float>&);
		void MulView(const TB::Matrix<4, 4, float>&);

		//レイヤーの登録
		// Note:近いレイヤーから登録すること
		void AddLayer(Object& layer);

		void RegisterStickies(Object&); // 顔張り付き物体の登録
		void RegisterObject(Object&); // 物体の登録
		void RegisterScenery(Scenery*); // Sceneryの登録

		//周期処理の入口
		void Run();

	protected:
		TB::Matrix<4, 4, float> view;
		virtual void Draw(const TB::Matrix<4, 4, float>&);
		virtual bool Finish() = 0;

	private:
		TB::List<Object> stickies;
		TB::List<Object> objects;
		Scenery* scenery;
	};
}
