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

#include <toolbox/tg/widget/border.h>



namespace TG {

	Widget::Found BorderWidget::Inside(const TB::Vector<2, float>& p) {
		Found f;
		if (p[0] < 0 || p[1] < 0) {
			return f;
		}

		TB::Vector<2, unsigned> r(size);
		r /= (unsigned)position[2];

		if (p[0] < r[0] && p[1] < r[1]) {
			f.depth = position[2];
			f.widget = this;
			f.where = TB::Vector<2, int>({(int)p[0], (int)p[1]});
		}
		return f;
	}

	void BorderWidget::CommonDraw() {
		const TB::Vector<3, float> rb(
			{position[0] + size[0], position[1] + size[1], position[2]});
		glColor4bv((GLbyte*)&color);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3fv(position);
		glVertex3i(rb[0], position[1], position[2]);
		glVertex3fv(rb);
		glVertex3f(position[0], rb[2], position[2]);
		glEnd();
	}
}
