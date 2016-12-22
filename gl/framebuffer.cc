/** Framebuffer object
 * Copyright (C) 2016 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/gl/gl.h>
#include <toolbox/gl/framebuffer.h>



namespace GL{

	Framebuffer::Framebuffer(
		unsigned width,
		unsigned height,
		TEXTURE::Format format=RGB) :
		TEXTURE(width, height, format),
		fbID(NewID()){
		Key k(*this);
		glFramebufferTexture2DEXT(
			GL_FRAMEBUFFER_EXT,
			GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, TextureID(), 0);
	};

	unsigned Framebuffer::NewID(){
		unsigned id;
		glGenRenderbuffersEXT(1, &id);
		return id;
	}


	/** Key
	 * RAIIによるアクティベート管理
	 */
	int Framebuffer::Key::activeID(0);

	Framebuffer::Key::Key(Framebuffer& fb){
		assert(!activeID);
		activeID = fb.fbID;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb.fbID);
	}
	Framebuffer::Key::~Key(){
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		activeID = 0;
	}



}
