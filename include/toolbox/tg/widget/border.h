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

#include <toolbox/tg/widget/position.h>
#include <toolbox/tg/widget/cursor.h>
#include <toolbox/geometry/spread.h>



namespace TG {

	class BorderWidget : public PositionWidget {
	public:
		BorderWidget(
			const TB::Vector<3, float>& position,
			const TB::Spread<2, unsigned>& size,
			unsigned color = 0,
			Widget* super = 0)
			: PositionWidget(position, super), size(size), state(Cursor::arrow),
			  trawCursor(DummyDrawCursor) {
			SetColor(color);
		};
		void SetColor(unsigned c) {
			color = c;
			if ((color & transparentMask) == transparentMask) {
				draw = &BorderWidget::CommonDraw;
				traw = &BorderWidget::DummyDraw;
			} else {
				traw = &BorderWidget::CommonDraw;
				draw = &BorderWidget::DummyDraw;
			}
		};

	protected:
		virtual void CommonDraw();
		void DummyDraw(){};

		void AtPointerEnter(const PointerEvent&) final;
		void AtPointerLeave(const PointerEvent&) final;
		void AtPointerMove(const PointerEvent&) final;

	private:
		static const unsigned transparentMask = 0xff000000;
		TB::Spread<2, unsigned> size;
		unsigned color;
		void (BorderWidget::*draw)();
		void (BorderWidget::*traw)();

		void Draw(const TB::Rect<2, float>&) final;
		void Traw(const TB::Rect<2, float>&) final;

		Found Find(const Query&) final;
		Found Inside(const Query&);

		// カーソル関連
		Cursor::State state;
		Cursor::TrawHandler trawCursor;
		static void DummyDrawCursor(Cursor::State){};
	};
}
