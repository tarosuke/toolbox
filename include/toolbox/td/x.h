/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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
#include <unordered_map>



namespace TB {
	namespace X {

		class Window;

		struct Display {
			friend class Window;

			Display(const char* target = 0);
			~Display();

			void Tick();

			static ::Display* xdisplay;
			static std::unordered_map<::Window, TB::X::Window*> dic;

		private:
			static void AddWindow(Window* w, ::Window xs) { dic[xs] = w; };
			static void RemoveWindow(::Window xw) { dic.erase(xw); };
			static Display instance;
		};

		struct Window {
			Window() : xwindow(0){}; //入れ物用何もしないコンストラクタ
			Window(unsigned width, unsigned height);
			Window(Window& w) noexcept { *this = w; };
			~Window();

			void operator=(Window& w) noexcept {
				xwindow = w.xwindow;
				w.xwindow = 0;
			};

			XWindowAttributes GetAttributes() const;

			::Window xwindow;
			static const long defaultEventMask = KeyPressMask | KeyReleaseMask |
												 ButtonPressMask |
												 ButtonReleaseMask;
		};
	}
}
