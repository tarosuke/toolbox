/** Path search
 * Copyright (C) 2018 tarosuke<webmaster@tarosuke.net>
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

#include <tb/string.h>



namespace tb {

	class Path : public String {
	public:
		Path() : String(){};
		Path(const char* path) : String(path){};

		const char* BaseName() { return Base((const char*)*this); };
		static const char* Base(const char*);


	protected:
		Path(const String&, const char*);
		Path(const String&, const String&);
		Path(const String&, const Path&);

	private:
		static String StatPath(const String&, const String&);
		static bool Is(const String&);
	};

}
