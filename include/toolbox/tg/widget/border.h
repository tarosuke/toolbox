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



namespace TG {
	class BorderWidget : public PositionWidget {
	public:
		BorderWidget(
			const TB::Vector<3, float>& position,
			const TB::Vector<2, int>& size,
			unsigned color = 0,
			Widget* super = 0)
			: PositionWidget(position, super), size(size) {
			SetColor(color);
		};
		void SetColor(unsigned c) {
			color = c;
			drawIt = (color & transparentMask) == transparentMask;
			trawIt = !drawIt && !!(~color & transparentMask);
		};

	protected:
		virtual void CommonDraw();

	private:
		static const unsigned transparentMask = 0xff000000;
		TB::Vector<2, unsigned> size;
		unsigned color;
		bool drawIt;
		bool trawIt;

		void Draw() final {
			if (drawIt) {
				CommonDraw();
			}
		};
		void Traw() final {
			if (trawIt) {
				CommonDraw();
			}
		};


		Found Inside(const TB::Vector<2, float>&);
	};
}