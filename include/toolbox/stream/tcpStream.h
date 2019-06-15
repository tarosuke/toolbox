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
#pragma once

#include "stream.h"



namespace TB{
	class TCPStream : public Stream{
	public:
		TCPStream(const char* host, unsigned port);
		~TCPStream();

		class Server{
			Server();
			Server(const Server&);
			void operator=(const Server&);
		public:
			Server(unsigned pert);
			Stream* Listen();

		protected:
			int fd;
		};

	protected:
		int fd;
		unsigned Read(void*, unsigned) override;
		unsigned Write(const void*, unsigned) override;

	private:
		TCPStream(int fd); // for listen only
	};
}
