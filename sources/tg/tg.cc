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



namespace TG {

	void Scene::Draw() {
		TB::Matrix<4, 4> mat;
		mat.Identity();

		for (TB::List<Object>::I i(layers); ++i;) {
			(*i).Draw(mat);
		}
	}

	void Group::Draw(const TB::Matrix<4, 4>& mat) {
		if (!visible) {
			return;
		}

		TB::Matrix<4, 4> matrix(mat * view);

		for (TB::List<Object>::I i(children); ++i;) {
			(*i).Draw(matrix);
		}
		for (TB::List<Object>::I i(groups); ++i;) {
			(*i).Draw(matrix);
		}
	}
}
