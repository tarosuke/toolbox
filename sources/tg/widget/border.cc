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
#include <toolbox/tg/widget/cursor.h>



namespace TG {

	Widget::Found BorderWidget::Inside(const Query& q) {
		const TB::Vector<2, int> p{
			(int)(q.pointer[0] * q.depth),
			(int)(q.pointer[1] * q.depth)};
		Found f;
		if (p[0] < 0 || p[1] < 0) {
			return f;
		}

		if ((unsigned)p[0] < size[0] && (unsigned)p[1] < size[1]) {
			f.depth = position[2];
			f.widget = this;
			f.where = p;
		}
		return f;
	}

	Widget::Found BorderWidget::Find(const Query& q) {
		const Query qq(NewQuery(q));

		Found f(Widget::Find(qq));
		return f.widget ? f : Inside(qq);
	}


	void BorderWidget::CommonDraw() {
		glColor4bv((GLbyte*)&color);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3i(0, 0, 0);
		glVertex2i(size[0], 0);
		glVertex2i(size[0], size[1]);
		glVertex2i(0, size[1]);
		glEnd();
	}

	void BorderWidget::Draw(const TB::Rect<2, float>& r) {
		glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		Widget::Draw(r - TB::Vector<2, float>((float*)position));
		if (drawIt) {
			CommonDraw();
		}
		glPopMatrix();
	};
	void BorderWidget::Traw(const TB::Rect<2, float>& r) {
		glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		if (trawIt) {
			CommonDraw();
		}
		Widget::Traw(r - TB::Vector<2, float>((float*)position));
		Cursor::Traw(this, state);
		glPopMatrix();
	};

	void BorderWidget::AtPointerEnter(const PointerEvent& e) {
		//カーソル登録
		Cursor::Enter(*this);

		//通常のイベント処理
		OnPointerEnter(e);
	}
	void BorderWidget::AtPointerLeave(const PointerEvent& e) {
		//カーソル抹消
		Cursor::Leave();

		//通常のイベント処理
		OnPointerEnter(e);
	}
	void BorderWidget::AtPointerMove(const PointerEvent& e) {
		//カーソル移動
		Cursor::SetPosition(e.position);

		//通常のイベント処理
		OnPointerMove(e);
	}
}
