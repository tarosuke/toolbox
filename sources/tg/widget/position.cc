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
#include <toolbox/tg/widget/position.h>



namespace TG {

	Widget::Found PositionWidget::Find(const Query& q) {
		TB::Vector<2, float> p({position[0], position[1]});
		p -= q.looknigPoint;
		p /= position[2];
		Query qq(q);
		qq.pointer -= p;

		Found f(Widget::Find(qq));

		if (!f.widget) {
			f = Inside(qq.pointer);
		}
		return f;
	}

	void PositionWidget::Draw() {
		glPopMatrix();
		glTranslatef(position[0], position[1], position[2]);
		Widget::Draw();
		glPopMatrix();
	}
	void PositionWidget::Traw() {
		glPopMatrix();
		glTranslatef(position[0], position[1], position[2]);
		Widget::Traw();
		glPopMatrix();
	}
}
