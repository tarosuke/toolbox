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

#include <toolbox/tg/tg.h>
#include <toolbox/geometry/rect.h>

#include <float.h>



namespace TG {

	class Widget : public TB::List<Widget>::Node {
		Widget(const Widget&);
		void operator=(const Widget&);

	public:
		//周期処理
		virtual void Draw(const TB::Rect<2, float>&); //引数は視錐体
		virtual void Traw(const TB::Rect<2, float>&);
		virtual void Tick();
		virtual ~Widget(){};

		//ボタン状態
		struct ButtonState {
			void operator=(unsigned newState) {
				pressed = newState & ~state;
				released = ~newState & state;
				state = newState;
			};
			unsigned state;
			unsigned pressed;
			unsigned released;
		};

		//イベント
		struct PointerEvent {
			const TB::Vector<2, int>& position;
			const ButtonState& button;
		};
		struct KeyEvent {
			unsigned keyCode;
			unsigned charCode;
		};

		virtual void AtPointerEnter(const PointerEvent&){};
		virtual void AtPointerLeave(const PointerEvent&){};
		virtual void AtPointerMove(const PointerEvent&){};

		virtual void OnPointerEnter(const PointerEvent&){};
		virtual void OnPointerLeave(const PointerEvent&){};
		virtual void OnPointerMove(const PointerEvent&){};
		virtual void OnButton(const PointerEvent&){};
		virtual void OnKeyDown(const KeyEvent&){};
		virtual void OnKeyUp(const KeyEvent&){};
		virtual void OnKeyRepeated(const KeyEvent&){};
		virtual void OnResized(const TB::Vector<2, unsigned>& p){};
		virtual bool MayClose() { return true; };

	protected:
		TB::List<Widget> subs;
		Widget(Widget* super);

		//窓の距離を考慮して仮想位置にあるポインタにかかっているWidgetを返す
		struct Query {
			const TB::Vector<2, float>& looknigPoint;
			TB::Vector<2, float> pointer;
			float depth;
		};
		struct Found {
			Found() : widget(0), depth(FLT_MAX){};
			Widget* widget;
			float depth;
			TB::Vector<2, int> where;
		};
		virtual Found Find(const Query&);

	private:
		static Widget* root;
	};
}
