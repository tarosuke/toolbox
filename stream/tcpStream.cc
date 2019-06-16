/** TCP Stream
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
#include <toolbox/stream/tcpStream.h>

#include <toolbox/exception/posixException.h>

#include <unistd.h>
#include <memory.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



namespace TB{
	TCPStream::TCPStream(const char* host, const char* service) : fd(-1){
		addrinfo* targets;
		addrinfo hints;

		// resolv hostname & service
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(host, service, &hints, &targets);

		// scan addresses
		for(addrinfo* t(targets); t; t = (*t).ai_next){
			if((fd = socket(
				(*t).ai_family,
				(*t).ai_socktype,
				(*t).ai_protocol)) < 0){
				// failed to create socket
				continue;
			}

			if(!!connect(fd, (*t).ai_addr, (*t).ai_addrlen)){
				// failed to connect
				close(fd);
				continue;
			}

			// connected
			freeaddrinfo(targets);
			return;
		}

		// failed to connect
		fd = -1;
		freeaddrinfo(targets);
		throw Exception();
	}
	TCPStream::TCPStream(int fd) : fd(fd){}

	TCPStream::~TCPStream(){
		if(0 <= fd){
			close(fd);
		}
	}

	unsigned TCPStream::Read(void* b, unsigned len){
		ssize_t r(recv(fd, b, len, 0));
		if(r < 0){
			throw PosixException();
		}
		return static_cast<unsigned>(r);
	}

	unsigned TCPStream::Write(const void* b, unsigned len){
		ssize_t r(send(fd, b, len, 0));
		if(r < 0){
			throw PosixException();
		}
		return static_cast<unsigned>(r);
	}

	TCPStream::Server::Server(const char* service){
	}

	Stream* TCPStream::Server::Listen(){
		return new TCPStream(JustListen());
	}
}
