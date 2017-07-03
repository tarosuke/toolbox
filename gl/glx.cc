/****************************************************************** GLXコンテキスト
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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
 *
 * GLXコンテキストのラッパ
 *
 */

#include <toolbox/gl/glx.h>


namespace TB{

	/** デフォルトのビジュアルフォーマット
	 */
	int GLX::defaultAttributes[] = {
		GLX_USE_GL,
		GLX_LEVEL, 0,
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_ACCUM_RED_SIZE, 0,
		GLX_ACCUM_GREEN_SIZE, 0,
		GLX_ACCUM_BLUE_SIZE, 0,
		GLX_ACCUM_ALPHA_SIZE, 0,
		None
	};



}
