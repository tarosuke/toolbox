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
#include <stdexcept>

#include <toolbox/td/x.h>



namespace TB {
	namespace X {

		::Display* Display::xdisplay;
		std::unordered_map<::Window, Window*> Display::dic;

		Display::Display(const char* target) {
			xdisplay = XOpenDisplay(target);
		}

		Display::~Display() {
			if (xdisplay) {
				XCloseDisplay(xdisplay);
			}
		}

		void Display::Tick() {}



		Window::Window(unsigned width, unsigned height)
			: xwindow(XCreateSimpleWindow(
				  Display::xdisplay,
				  XDefaultRootWindow(Display::xdisplay),
				  0,
				  0,
				  width,
				  height,
				  0,
				  0,
				  0)) {
			if (!xwindow) {
				throw std::runtime_error("Failed to open X Window.");
			}
			if (XMapWindow(Display::xdisplay, xwindow) < 0 ||
				XSelectInput(Display::xdisplay, xwindow, defaultEventMask) <
					0) {
				XDestroyWindow(Display::xdisplay, xwindow);
				xwindow = 0;
				throw std::runtime_error("Failed to open X Window.");
			}
		}
		Window::~Window() { XDestroyWindow(Display::xdisplay, xwindow); }
	}
}
