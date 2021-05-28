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
#include <toolbox/tg/tg.h>
#include <toolbox/tg/object.h>
#include <toolbox/tg/scenery.h>
#include <toolbox/gl/gl.h>

#include <assert.h>


namespace TG {

	Scene::Scene() : scenery(0) { view.Identity(); }
	Scene::~Scene() {
		if (scenery) {
			delete scenery;
		}
	}

	void Scene::SetFrustum(const Frustum& frustum) {
		glMatrixMode(GL_PROJECTION);
		glFrustum(
			frustum.left,
			frustum.right,
			frustum.bottom,
			frustum.top,
			frustum.near,
			frustum.far);
		glMatrixMode(GL_MODELVIEW);
	}

	void Scene::SetProjectionMatrix(const double projectionMatrix[]) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(projectionMatrix);
		glMatrixMode(GL_MODELVIEW);
	}

	void Scene::RegisterStickies(Object& o) { stickies.Add(o); }
	void Scene::RegisterObject(Object& o) { objects.Add(o); }
	void Scene::RegisterScenery(Scenery* s) {
		if (scenery) {
			delete scenery;
		}
		scenery = s;
	}

	void Scene::Run() {
		// GLのモード設定
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		do {
			Draw(view);
			stickies.Foreach(&Object::Tick);
			objects.Foreach(&Object::Tick);
		} while (Finish());
	}

	void Scene::Draw(const TB::Matrix<4, 4, float>& v) {
		//フレームバッファクリアモード
		static const unsigned clearAll(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		static const unsigned clear(
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		static unsigned clearFlags(clearAll);

		glClearColor(0, 0, 0.1, 1);
		glClear(clearFlags);
		glColor3f(1, 1, 1);

		glLoadIdentity();
		stickies.Foreach(&Object::Draw);

		//カメラの反映
		glLoadMatrixf(v);
		objects.Foreach(&Object::Draw);
		if (scenery) {
			(*scenery).Draw();
			clearFlags = clear;
		} else {
			clearFlags = clearAll;
		}
		objects.Reveach(&Object::Traw);

		glLoadIdentity();
		stickies.Reveach(&Object::Traw);
	}
}
