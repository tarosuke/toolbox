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
#include <toolbox/tg/object.h>
#include <GL/glew.h>
#include <GL/gl.h>



namespace TG {

	class OMesh : public Mesh {
	public:
		void Draw() override { DrawMesh(); };
		OMesh(TB::VBO* vbo, TB::Texture* texture) : Mesh(vbo, texture){};
	};

	class TMesh : public Mesh {
	public:
		void Traw() override { DrawMesh(); };
		TMesh(TB::VBO* vbo, TB::Texture* texture) : Mesh(vbo, texture){};
	};

	Mesh* Mesh::New(TB::VBO* vbo, TB::Texture* texture) {
		if (!vbo || !texture) {
			return 0;
		}
		return (*texture).IsTransparent() ? (Mesh*)new TMesh(vbo, texture)
										  : (Mesh*)new OMesh(vbo, texture);
	}

	void Mesh::DrawMesh() {
		//カラーバッファのセットアップ
		TB::Texture::Binder b(*texture);

		//描画
		(*vbo).Draw();
	}


	void Group::Draw() {
		glPushMatrix();
		glMultMatrixf(matrix);
		for (TB::List<Object>::I i(children); ++i;) {
			(*i).Draw();
		}
		for (TB::List<Object>::I i(groups); ++i;) {
			(*i).Draw();
		}
		glPopMatrix();
	}
	void Group::Traw() {
		glPushMatrix();
		glMultMatrixf(matrix);
		for (TB::List<Object>::I i(groups); --i;) {
			(*i).Traw();
		}
		for (TB::List<Object>::I i(children); --i;) {
			(*i).Traw();
		}
		glPopMatrix();
	}
	void Group::Tick() {
		for (TB::List<Object>::I i(children); ++i;) {
			(*i).Tick();
		}
		for (TB::List<Object>::I i(groups); ++i;) {
			(*i).Tick();
		}
	}



}
