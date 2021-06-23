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

#include <toolbox/tg/widget.h>



namespace TG {
	class PositionWidget : public Widget {
		PositionWidget();
		PositionWidget(const PositionWidget&);
		void operator=(const PositionWidget&);

	public:
		PositionWidget(const TB::Vector<3, float>& p, Widget* super = 0)
			: Widget(super), position(p), targetPosition(p){};

		template <unsigned E, typename U>
		void MoveTo(const TB::Vector<E, U>& to) {
			targetPosition = to;
		};
		template <unsigned E, typename U>
		void JumpTo(const TB::Vector<E, U>& to) {
			position = targetPosition = to;
		};


	protected:
		TB::Vector<3, float> position;

		void Tick() override;
		void Draw(const TB::Rect<2, float>&) override;
		void Traw(const TB::Rect<2, float>&) override;

		Found Find(const Query&) override;
		Query NewQuery(const Query&); //自座標系(みかけ)に変換

	private:
		static float followRatio;
		TB::Vector<3, float> targetPosition;
	};
}
