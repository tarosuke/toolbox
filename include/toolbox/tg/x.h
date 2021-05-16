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

#include <toolbox/container/list.h>
#include <toolbox/geometry/vector.h>

#include <X11/Xlib.h>
#include <assert.h>



namespace XTG {
	class Window;

	class Display : public TB::List<Window> {
		friend class Window;

	public:
		Display(const char* target = NULL) : xdisplay(XOpenDisplay(target)) {
			current = this;
		}
		virtual ~Display() {
			if (xdisplay) {
				XCloseDisplay(xdisplay);
			}
		}

		//導出してこれを置き換えて使う
		virtual void HandleEvent(const XEvent&) {}

		// NOTE:Xはイベントを取りに行かないと処理が進まないので一度は呼ぶこと
		void Run() {
			if (xdisplay) {
				XEvent ev;
				::XNextEvent(xdisplay, &ev);
				HandleEvent(ev);
			}
		};

		operator bool() { return !!xdisplay; }

		static Display& Current() {
			assert(current);
			return *current;
		}

	private:
		::Display* const xdisplay;
		static Display* current;
	};


	class Window : public TB::List<Window>::Node {
	public:
		Window(unsigned width, unsigned height, Window* parent = 0);
		~Window() { XDestroyWindow(display.xdisplay, xdrawable); }

		TB::Vector<2, unsigned> Size() const { return size; };
		bool IsReady() const { return 0 < xdrawable; };

	protected:
		::Display* XDisplay() const { return display.xdisplay; }
		::Window XWindow() const { return xdrawable; }

	private:
		Display& display;
		::Window const xdrawable;
		TB::Vector<2, unsigned> size;

		static const long defaultEventMask;
	};
}
