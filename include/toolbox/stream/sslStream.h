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

/** Usage for client socket
 * 1. Make an instance of SSLStream and wait
 * 2. The instance is the stream
 * 2a. It throws Exception when failed
 *
 ** Usage for server socket
 * 1. Make an instace of SSLStream::Server
 * 2. Listen the instance
 * 3. It returns the stream when it was connected
 * 3a. It throws Exception when it gets some failure
 */
#pragma once

#include <openssl/ssl.h>

#include "tcpStream.h"



namespace TB{
	class SSLStream : public TCPStream{
	public:
		SSLStream(const char* target, const char* service);
		~SSLStream();

		// server socket
		class Server : public TCPStream::Server{
		public:
			Server(const char* service);
			Stream* Listen();
		};

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

		void Attach();
		void ShutDown();
		void Quit();

		SSLStream(int fd);
	};
}
