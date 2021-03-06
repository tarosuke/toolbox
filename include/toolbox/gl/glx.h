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
 * 構築したらMakeCurrentでカレントへ
 *
 */
#pragma once

#include <toolbox/gl/gl.h>
#include <X11/Xlib.h>


namespace TB{

	class GLX{
		GLX();
		void operator=(const GLX&);
	public:
		/** 通常構築子
		 * 何も指定しなければ根窓とデフォルトビジュアルで生成
		 */
		GLX(Display* display, Drawable drawable = 0, int attributes[] = defaultAttributes) :
			display(display),
			drawable(drawable ? drawable : DefaultRootWindow(display)),
			child(false){
			visual = glXChooseVisual(
				display, DefaultScreen(display), attributes);
			context = glXCreateContext(display, visual, NULL, True);
		};

		/* 子コンテキスト構築子
		 * 親コンテキストとリソースを共有する場合に使う
		 * drawableを指定しなければ親コンテキストと同じdrawableが使われる
		 *
		 * なお、Threadと併用するときはThreadより前に初期化すること！
		 */
		GLX(const GLX& parent, Drawable drawable = 0) :
			display(parent.display),
			drawable(drawable ? drawable : parent.drawable),
			visual(parent.visual),
			context(glXCreateContext(
				display, visual, parent.context, True)),
			child(true){
		};

		/** デストラクタ
		 */
		~GLX(){
			if(!child){
				XFree(visual);
			}
			glXMakeCurrent(display, None, NULL); //コンテキスト解除
			glXDestroyContext(display, context); //コンテキスト破棄
		};

		/** コンテキストをカレントにする
		 * スレッドを使う場合、構築は親スレッド、MakeCurrentは子スレッドで行う必要がある
		 */
		void MakeCurrent(){
			glXMakeCurrent(display, drawable, context);
		};

		Display* GetDisplay(){ return display; };

	private:
		static int defaultAttributes[];

		Display* display;
		Drawable drawable;
		XVisualInfo* visual;
		GLXContext context;
		const bool child;

	};

}

