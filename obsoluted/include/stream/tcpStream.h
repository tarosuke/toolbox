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

/** Usage for client socket
 * 1. Make an instance of TCPStream and wait
 * 2. The instance is the stream
 * 2a. It throws Exception when failed
 *
 ** Usage for server socket
 * 1. Make an instace of TCPStream::Server
 * 2. Listen the instance
 * 3. It returns the stream when it was connected
 * 3a. It throws Exception when it gets some failure
 */
#pragma once

#include "stream.h"



namespace TB{
	class TCPStream : public Stream{
	public:
		TCPStream(const char* host, const char* service);
		~TCPStream();

		// server socket
		class Server{
			Server();
			Server(const Server&);
			void operator=(const Server&);
		public:
			Server(const char* service);
			Stream* Accept();

		protected:
			int fd;

			int JustAccept(); //just accept and retcun socket
		};

	protected:
		int fd;
		unsigned Read(void*, unsigned) override;
		unsigned Write(const void*, unsigned) override;

		TCPStream(int fd);
	};
}
