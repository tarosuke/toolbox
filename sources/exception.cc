/** Exception
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
#include <stdlib.h>
#include <string.h>
#include <string>
#include <toolbox/exception.h>



namespace TB {
	Exception::Exception(const Exception& e) {
		char* message((char*)malloc(strlen(e.message)));
		strcpy(message, e.message);
	};
	Exception::Exception(const char* m, const char* p, unsigned l) {
		std::string mm;
		if (p) {
			mm += p;
			mm += " : " + std::to_string(l);
		}
		mm += " :: ";
		mm += m;
		message = (char*)malloc(mm.length());
		strcpy(message, mm.c_str());
	}
	Exception::~Exception() {
		if (message) {
			free(message);
		};
	}

	PosixException::PosixException(const char* path, unsigned line)
		: Exception(strerror(errno), path, line) {}
}
