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
#include <toolbox/gl/gl.h>
#include <toolbox/tg/widget/cursor.h>



namespace TG {

	TG::Cursor* TG::Cursor::instance(0);
	TB::Vector<2, int> Cursor::position;

	void Cursor::Traw(State state) {
		if (instance) {
			TB::Texture::Binder b((*instance).texture);
			glColor3f(1, 1, 1);
			glPushMatrix();
			glTranslatef(position[0], position[1], 0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 0, -0.001);
			glVertex3f(0.16, 0, -0.001);
			glVertex3f(0.16, 0.16, -0.001);
			glVertex3f(0, 0.16, -0.001);
			glEnd();
			glPopMatrix();
		}
	}

	void TG::Cursor::New(const char* path) {
		if (auto* const image = TB::Image::New(path)) {
			new Cursor(*image);
			delete image;
		}
	}


	TG::Cursor::Cursor(TB::Image& image)
		: texture(image),
		  coordsSize((const float[2]){
			  32.0f / image.GetWidth(), 32.0f / image.GetHeight()}) {
		if (instance) {
			delete instance;
		}
		instance = this;
		}
}
