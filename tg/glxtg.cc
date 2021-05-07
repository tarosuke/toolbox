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
#include <toolbox/tg/glxtg.h>
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>



namespace {
	class GLX {
	public:
		GLX() : display(XOpenDisplay(NULL)) {
			if (!display) {
				return;
			}
			root = DefaultRootWindow(display);
		}
		void SetVisual(int attributes[]) {
			attributes = attributes;
			visual = glXChooseVisual(display, root, attributes);
			context = glXCreateContext(display, visual, NULL, True);
		}
		~GLX() {}

	private:
		Display* const display;
		Drawable root;
		int* attributes;
		XVisualInfo* visual;
		GLXContext context;
	} glx;

}



namespace TG {

	GLXScene::GLXScene(const Frustum& frustum, int attributes[])
		: Scene(frustum) {
		glx.SetVisual(attributes);
	}
	GLXScene::GLXScene(const double projectionMatrix[], int attributes[])
		: Scene(projectionMatrix) {
		glx.SetVisual(attributes);
	}



	/** デフォルトのビジュアルフォーマット
	 */
	int GLXScene::defaultAttributes[] = {
		GLX_USE_GL,
		GLX_LEVEL,
		0,
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_RED_SIZE,
		8,
		GLX_GREEN_SIZE,
		8,
		GLX_BLUE_SIZE,
		8,
		GLX_ALPHA_SIZE,
		8,
		GLX_DEPTH_SIZE,
		24,
		GLX_STENCIL_SIZE,
		8,
		GLX_ACCUM_RED_SIZE,
		0,
		GLX_ACCUM_GREEN_SIZE,
		0,
		GLX_ACCUM_BLUE_SIZE,
		0,
		GLX_ACCUM_ALPHA_SIZE,
		0,
		None};

}
