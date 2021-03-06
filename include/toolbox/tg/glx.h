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
#include <toolbox/tg/x.h>
#include <toolbox/gl/gl.h>


namespace XTG {
	class Display;
}


namespace TG {

	class GLXScene : public XTG::Window, public Scene {
		GLXScene(const GLXScene&);
		void operator=(const GLXScene&);

	public:
		GLXScene(
			XTG::Display&,
			unsigned width,
			unsigned height,
			XTG::Window* parent,
			const Frustum&,
			int attirbutes[] = defaultAttributes);

	protected:
		void Quit() { keep = false; };

	private:
		static int defaultAttributes[];
		int* attributes;
		XVisualInfo* visual;
		GLXContext context;
		bool keep;

		void Init(int* attributes);
		bool Finish(unsigned) final;
	};
}
