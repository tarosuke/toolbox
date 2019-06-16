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
#include <toolbox/stream/sslStream.h>

#include <sys/socket.h>
#include <openssl/err.h>
#include <unistd.h>



#define assert(c) { if(!(c)){ ShutDown(); } }

namespace TB{
	SSL_CTX* SSLStream::ctx(0);

	SSLStream::SSLStream(const char* host, const char* service) :
			TCPStream(host, service),
			ssl(0){
		Attach();
	}

	SSLStream::SSLStream(int fd) : TCPStream(fd){
		Attach();
	}

	SSLStream::~SSLStream(){
		Quit();
	}

	void SSLStream::Attach(){
		assert(ssl = SSL_new(ctx));
    	assert(SSL_set_fd(ssl, fd) == 1);
		assert(SSL_connect(ssl)  == 1);
	}

	void SSLStream::ShutDown(){
		Quit();
		throw Exception();
	}
	void SSLStream::Quit(){
		if(ssl){
			SSL_shutdown(ssl);
			SSL_free(ssl);
			ssl = 0;
		}
	}

	unsigned SSLStream::Read(void* b, unsigned len){
		const int r(SSL_read(ssl, b, len));
		if(r < 0){
			throw Exception();
		}
		return static_cast<unsigned>(r);
	}

	unsigned SSLStream::Write(const void* b, unsigned len){
		const int r(SSL_write(ssl, b, len));
		if(r < 0){
			throw Exception();
		}
		return static_cast<unsigned>(r);
	}

	SSLStream::Server::Server(const char* service) :
		TCPStream::Server(service){}

	Stream* SSLStream::Server::Accept(){
		return new SSLStream(JustAccept());
	}

	SSLStream::Exception::operator const char*(){
		const unsigned long e(ERR_get_error());
		return ERR_reason_error_string(e);
	}

	SSLStream::Init::Init(){
	    SSL_library_init();
		SSL_load_error_strings();
		ctx = SSL_CTX_new(SSLv23_method());
	}
	SSLStream::Init::~Init(){
		ERR_free_strings();
	}
}
