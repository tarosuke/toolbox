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
		TEXTURE::Format format) :
		TEXTURE(width, height, format),
		fbID(NewID()),
		dbID(NewDB()){
		Key k(*this);

		//デプスバッファ確保
		glBindRenderbuffer(
			GL_RENDERBUFFER,
			dbID);
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			GL_DEPTH_COMPONENT,
			width,
			height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//カラーバッファ割り当て
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, TextureID(), 0);

		//デプスバッファ割り当て
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, dbID);
	}

	unsigned Framebuffer::NewID(){
		unsigned id(0);
		glGenFramebuffers(1, &id);
		return id;
	}
	unsigned Framebuffer::NewDB(){
		unsigned id(0);
		glGenRenderbuffers(1, &id);
		return id;
	}



	Framebuffer::~Framebuffer(){
		glDeleteFramebuffers(1, &fbID);
		glDeleteRenderbuffers(1, &dbID);
	}


	/** Key
	 * RAIIによるアクティベート管理
	 */
	int Framebuffer::Key::activeID(0);

	Framebuffer::Key::Key(Framebuffer& fb){
		assert(!activeID);
		activeID = fb.fbID;
		glBindFramebuffer(GL_FRAMEBUFFER, fb.fbID);
	}
	Framebuffer::Key::~Key(){
		assert(activeID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		activeID = 0;
	}



}
