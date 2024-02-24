/**
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



namespace TB {

	class Socket {
		Socket();
		Socket(const Socket&);
		void operator=(const Socket&);

	public:
		Socket(int);
		virtual ~Socket();

	protected:
		virtual int Read(void*, int);
		virtual int Write(const void*, int);

		const int fd;
	};


	class SSLSocket : public Socket {
		SSLSocket();
		SSLSocket(const SSLSocket&);
		void operator=(const SSLSocket&);

	public:
		SSLSocket(int, SSL_CTX*);
		~SSLSocket();

	protected:
		int Read(void*, int) final;
		int Write(const void*, int) final;

	private:
		SSL* ssl;
	};



	class TCPServer {
		TCPServer();
		TCPServer(const TCPServer&);
		void operator=(const TCPServer&);

	public:
		TCPServer(unsigned port);
		~TCPServer();
		Socket* Accept();

	protected:
		int RawAccept();

	private:
		int sock;
	};



	class SSLServer : private TCPServer {
		SSLServer();
		SSLServer(const SSLServer&);
		void operator=(const SSLServer&);

	public:
		SSLServer(unsigned port);
		~SSLServer();
		Socket* Accept();

	private:
		SSL_CTX* ctx;

		static SSL_CTX* CreateContext();
	};
}
