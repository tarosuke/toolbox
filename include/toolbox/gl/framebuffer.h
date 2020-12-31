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

#pragma once

#include "texture.h"



namespace TB{

	class Framebuffer : public Texture{
		Framebuffer();
		Framebuffer(const Framebuffer&);
		void operator=(const Framebuffer&);
	public:

		/** Size
		 * サイズ
		 */
		struct Size{
			unsigned width;
			unsigned height;
		};

		/** Key
		 * RAIIによるアクティベート管理
		 */
		class Key{
			Key();
			Key(const Key&);
			void operator=(const Key);
		public:
			Key(Framebuffer&);
			~Key();
		private:
		};

		/** CancelKey
		 * RAIIによる一時的な解除
		 */
		class CanKey{
		public:
			CanKey();
			~CanKey();
		};


		Framebuffer(
			unsigned width,
			unsigned height,
			Format=Texture::RGB,
			bool withDepth=true);
		Framebuffer(
			Size,
			Format=Texture::RGB,
			bool withDepth=true);
		~Framebuffer();

		unsigned GetFBID() const { return fbID; };
		unsigned GetColorBufferID() const { return TextureID(); };
		unsigned GetDepthBufferID() const { return dbID; };

	private:
		const unsigned fbID;
		const unsigned dbID;
		static unsigned NewID();
		static unsigned NewDB();
		static int activeID;

		void Assign(
			unsigned width,
			unsigned height,
			Format,
			bool withDepth);
	};


};
