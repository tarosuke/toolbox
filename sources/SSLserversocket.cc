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

#include <toolbox/SSLserversocket.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/err.h>



namespace TB{

	Socket::Socket(int fd) : fd(fd){}
	Socket::~Socket(){ close(fd); }

	int Socket::Read(void* buff, int len){
		return read(fd, buff, len);
	}

	int Socket::Write(const void* buff, int len){
		return write(fd, buff, len);
	}


	SSLSocket::SSLSocket(int fd, SSL_CTX* ctx) : Socket(fd){
		ssl = SSL_new(ctx);
        SSL_set_fd(ssl, fd);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
	}

	int SSLSocket::Read(void* b, int len){
        return SSL_read(ssl, b, len);
    }

	int SSLSocket::Write(const void* b, int len){
        return SSL_write(ssl, b, len);
    }

	SSLSocket::~SSLSocket(){
		if(ssl){
			SSL_free(ssl);
		}
		if(0 <= fd){
        	close(fd);
		}
	}






	TCPServer::TCPServer(unsigned port){
		struct sockaddr_in addr;

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			throw "Failed to create server socket.";
		}

		if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
			throw "Failed to bind server socket.";
		}

		if(listen(sock, 3) < 0){
			throw "Failed to listen server socket.";
		}
	}

	int TCPServer::RawAccept(){
		struct sockaddr_in addr;
		uint len(sizeof(addr));

		const int fd(accept(sock, (struct sockaddr*)&addr, &len));
        if(fd < 0){
			throw "Failed to accept server socket";
        }
		return fd;
	}

	TCPServer::~TCPServer(){
		close(sock);
	}



	SSLServer::SSLServer(unsigned port) :
		TCPServer(port),
		ctx(CreateContext()){
		//SSLコンテキストの設定
		SSL_CTX_set_ecdh_auto(ctx, 1);

	    /* Set the key and cert */
	    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			throw "";
	    }

	    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
			ERR_print_errors_fp(stderr);
			throw "";
	    }
	}

	SSL_CTX* SSLServer::CreateContext(){
	    const SSL_METHOD *m(SSLv23_server_method());
	    SSL_CTX *ctx;

	    if(!(ctx = SSL_CTX_new(m))){
			throw "Failed to create SSL context.";
		}

	    return ctx;
	}

	Socket* SSLServer::Accept(){
		return new SSLSocket(TCPServer::RawAccept(), ctx);
	}

	SSLServer::~SSLServer(){
	    SSL_CTX_free(ctx);
		EVP_cleanup();
	}

}
