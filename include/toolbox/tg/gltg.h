/************************************************************ toolbox graphics
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/geometry/matrix.h>
#include <toolbox/tg/tg.h>
#include <toolbox/container/list.h>
#include <GL/glew.h>
#include <GL/gl.h>



namespace TG {

	class GLObject : public TB::List<GLObject>::Node {
	public:
		virtual void Draw(){};
		virtual void DrawTransparenrt(){};
		virtual void Tick(){};
	};

	class GLGroup : public GLObject {
	public:
		void AddChild(GLObject& o) { children.Add(o); };
		void AddCHild(GLGroup& g) { groups.Add(g); };

	protected:
		TB::List<GLObject> children;
		TB::List<GLObject> groups;

	private:
		TB::Matrix<4, 4, float> matrix;
		void SetMatrix(const TB::Matrix<4, 4, float>& m) { matrix = m; }
		void MulMatrix(const TB::Matrix<4, 4, float>& m) {
			matrix = matrix * m;
		}
		void Draw() final;
		void DrawTransparenrt() final;
		void Tick() final;
	};


	class GLScene : public Scene {
		GLScene(const GLScene&);
		void operator=(const GLScene&);

		//レイヤーの登録
		// Note:近いレイヤーから登録すること
		void AddLayer(GLObject& layer) { layers.Add(layer); };

	protected:
		GLScene(){};
		void SetFrustum(const Frustum& frustum);
		void SetProjectionMatrix(const double projectionMatrix[]);

		void Draw() override;
		void Tick() override;

	private:
		TB::List<GLObject> layers;
	};
}
