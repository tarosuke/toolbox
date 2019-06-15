/** SSL Stream
 * Copyright (C) 2019 tarosuke<webmaster@tarosuke.net>
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

#include <openssl/ssl.h>

#include "tcpStream.h"



namespace TB{
	class SSLStream : public TCPStream{
	public:
		SSLStream(const char* target, unsigned port);
		~SSLStream();

		/** Listen the port
		 * NOTE: DO NOT FORGET THIS IS BLOCKING INTERFACE
		 */
		static Stream* Listen(unsigned port);

	protected:
		unsigned Read(void*, unsigned) override;
		unsigned Write(const void*, unsigned) override;

	private:
		class Exception : public TB::Exception{
		public:
			operator const char*() final;

		private:
		};

		class Init{ Init(); ~Init(); };

		SSL* ssl;
		static SSL_CTX* ctx;

		void ShutDown();
		void Quit();
	};
}
