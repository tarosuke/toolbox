/** Stream
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
#include <toolbox/stream/stream.h>
#include <toolbox/exception/posixException.h>



namespace TB {

	// read a line to String
	Stream& Stream::operator>>(String& s) {
		for (char b('\n'); Read(&b, 1) && b != '\n';) {
			// append the charactor to string
			s << b;
		}

		return *this;
	}

	// write String
	Stream& Stream::operator<<(const String& s) {
		*this << s;
		return *this;
	}

	// write null-terminated string
	Stream& Stream::operator<<(const char* s) {
		*this << s;
		return *this;
	}

	Stream& Stream::operator<<(const endl&) {
		*this << '\n';
		Flush();
		return *this;
	}

	// flush writebuffer
	void Stream::Flush() {
		if (Length()) {
			Write(static_cast<const char*>(*this), Length());
			Clear();
		}
	}

}
