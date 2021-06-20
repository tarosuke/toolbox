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
#include <toolbox/input/input.h>
#include <toolbox/tg/widget/cursor.h>



namespace TG {

	class RootWidget : Widget, TB::Input {
		RootWidget(const RootWidget&);
		void operator=(const RootWidget&);

	public:
		RootWidget() : Widget(0), Input(false), bridge(*this){};

	private:
		static const float scale;
		static const float navigationRadious;
		static const TB::Rect<2, float> viewRect;

		TB::Vector<2, float> lookingPoint;
		Found prev;

		void Tick() final;
		void CalcLoockingPoint();

		// Widgetを使わない時に何もリンクしないようにするための折り返し点
		class Bridge : Scene::Object {
		public:
			Bridge(RootWidget& root) : root(root) {
				Scene::RegisterRoot(*this);
			};

		private:
			RootWidget& root;
			void Tick() final { root.Tick(); };
			void Draw() final;
			void Traw() final;
		} bridge;

		// カーソル関連
		static Cursor::TrawHandler trawCursor;
		static void DummyTrawCursor(Cursor::State){};

		//入力関連
		TB::Vector<2, float> pointer;
		ButtonState button;
		void EmitEvent();
		void OnKeyDown(unsigned key) final;
		void OnKeyUp(unsigned key) final;
		void OnKeyRepeat(unsigned key) final;
		void OnButtonDown(unsigned) final;
		void OnButtonUp(unsigned) final;
		void OnMouseMove(unsigned, int) final;
	};
}
