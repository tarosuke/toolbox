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
#include <GL/glew.h>
#include <GL/gl.h>



namespace TG {

	void Scene::SetFrustum(const Frustum& frustum) {
		glFrustum(
			frustum.left,
			frustum.right,
			frustum.bottom,
			frustum.top,
			frustum.near,
			frustum.far);
	}

	void Scene::SetProjectionMatrix(const double projectionMatrix[]) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(projectionMatrix);
	}

	void Scene::AddLayer(Object& layer) { layers.Add(layer); };


	void Scene::Draw() {
		glClearColor(0, 0, 0.1, 1);
		glClear(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		for (TB::List<Object>::I i(layers); ++i;) {
			(*i).Draw();
		}
		for (TB::List<Object>::I i(layers); --i;) {
			(*i).DrawTransparenrt();
		}
	}
	void Scene::Tick() {
		for (TB::List<Object>::I i(layers); ++i;) {
			(*i).Tick();
		}
	}
}
