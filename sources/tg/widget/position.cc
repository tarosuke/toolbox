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

	Widget::Query PositionWidget::NewQuery(const Query& q) {
		Query qq(q);
		qq.depth += position[2];
		TB::Vector<2, float> p(
			{position[0] / qq.depth, position[1] / qq.depth});
		qq.looknigPoint -= p;
		qq.pointer -= p;
		return qq;
	}

	Widget::Found PositionWidget::Find(const Query& q) {
		const Query qq(NewQuery(q));

		//子要素を検索
		return Widget::Find(qq);
	}

	void PositionWidget::Draw(const TB::Rect<2, float>& vr) {
		glPopMatrix();
		glTranslatef(position[0], position[1], position[2]);
		Widget::Draw(vr - TB::Vector<2, float>{position[0], position[1]});
		glPopMatrix();
	}
	void PositionWidget::Traw(const TB::Rect<2, float>& vr) {
		glPopMatrix();
		glTranslatef(position[0], position[1], position[2]);
		Widget::Traw(vr - TB::Vector<2, float>{position[0], position[1]});
		glPopMatrix();
	}
}
