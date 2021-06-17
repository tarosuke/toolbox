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

#include <toolbox/geometry/vector.h>
#include <toolbox/image.h>
#include <toolbox/gl/texture.h>



namespace TG {

	class Widget;

	class Cursor {
		Cursor(const Cursor&);
		void operator=(const Cursor&);

	public:
		enum State {
			none,
			arrow,
			busy,
		};

		// 新しいのをNewできたら古いのは置き換えられる
		static void New(const char* path);

		// カーソル操作
		static void Traw(State);
		static void SetPosition(const TB::Vector<2, int>& p) { position = p; };

	private:
		static TB::Vector<2, int> position; //基準面上の位置
		static Cursor* instance;

		Cursor(const char*);
		TB::Texture texture;
	};
}
