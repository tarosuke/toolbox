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



namespace TG {

	class Widget : public Scene::Object {
		Widget(const Widget&);
		void operator=(const Widget&);

	public:
		//周期処理
		virtual void Draw();
		virtual void Traw();
		virtual void Tick();
		virtual ~Widget(){};

		//イベント
		struct PointerEvent {
			TB::Vector<2, int> position;
			unsigned button;
			unsigned pressed;
			unsigned released;
			Widget* target;
		};
		struct KeyEvent {
			enum { pressed, released, repeated } event;
			unsigned keyCode;
			unsigned charCode;
		};

		virtual void OnPointerEnter(const PointerEvent& e){};
		virtual void OnPointerLeave(const PointerEvent& e){};
		virtual void OnPointerMove(const PointerEvent& e){};
		virtual void OnButtonDown(const PointerEvent& e){};
		virtual void OnButtonUp(const PointerEvent& e){};
		virtual void OnKeyDown(const KeyEvent& e){};
		virtual void OnKeyUp(const KeyEvent& e){};
		virtual void OnResized(const TB::Vector<2, unsigned>& p){};
		virtual bool MayClose() { return true; };

	protected:
		TB::List<Widget> subs;
		Widget() {}

		//窓の距離を考慮して仮想位置にあるポインタにかかっているWidgetを返す
		struct Query {
			const TB::Vector<2, float>& looknigPoint;
			TB::Vector<2, float> pointer;
			float depth;
		};
		struct Found {
			Widget* widget;
			float depth;
		};
		virtual Found Find(const Query&);
	};
}
