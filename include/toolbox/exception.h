/***** 例外
 *  Copyright (C) 2021, 2023 tarosuke<webmaster@tarosuke.net>
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



namespace TB {
	struct Exception {
		Exception() : message(0){};
		Exception(const Exception& e);
		Exception(const char* message, const char* path = 0, unsigned line = 0);
		~Exception();
		operator const char*() { return message ? message : ""; };

	protected:
		char* message;
	};

	class PosixException : Exception {
	public:
		PosixException();
		PosixException(const char* path = 0, unsigned line = 0);
	};
}

#define Posit(c)                                                               \
	if (!c) {                                                                  \
		throw TB::Exception("fail: Posit failed - " #c, __FILE__, __LINE__);   \
	}
