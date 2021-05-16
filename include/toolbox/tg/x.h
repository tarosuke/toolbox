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

#include <X11/Xlib.h>



namespace XTG {
	class Display {
		friend class Window;

	public:
		Display(const char* target = NULL) : xdisplay(XOpenDisplay(target)) {}
		~Display() {
			if (xdisplay) {
				XCloseDisplay(xdisplay);
			}
		}

		// NOTE:Xはイベントを取りに行かないと処理が進まない
		XEvent NextEvent() {
			XEvent ev;
			::XNextEvent(xdisplay, &ev);
			return ev;
		};

		operator bool() { return !!xdisplay; }

	private:
		::Display* const xdisplay;
	};

	class Window {
	public:
		Window(Display& d, unsigned width, unsigned height, Window* parent = 0);
		~Window() { XDestroyWindow(display.xdisplay, xdrawable); }

		operator bool() { return 0 < xdrawable; }

	private:
		Display display;
		::Window const xdrawable;
	};
}
