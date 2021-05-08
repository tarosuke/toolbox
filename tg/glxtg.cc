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
#include <assert.h>

#include <toolbox/tg/glxtg.h>



namespace TG {

	GLXScene::GLXScene(
		const Target& target, const Frustum& frustum, int attributes[])
		: target(target) {
		assert(target.display);
		Init(attributes);
		SetFrustum(frustum);
	}
	GLXScene::GLXScene(
		const Target& target, const double projectionMatrix[], int attributes[])
		: target(target) {
		assert(target.display);
		Init(attributes);
		SetProjectionMatrix(projectionMatrix);
	}

	void GLXScene::Init(int* attributes) {
		visual = glXChooseVisual(target.display, target.drawable, attributes);
		context = glXCreateContext(target.display, visual, NULL, True);
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
