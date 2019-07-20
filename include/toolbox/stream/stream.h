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
#pragma once

#include <toolbox/exception/exception.h>
#include <toolbox/string.h>



namespace TB{
	/** Stream
	 * Interface of Stream series
	 */
	class Stream : public String{
		Stream(const Stream&);
		void operator=(const Stream&);
	public:
		// manipulators
		class endl;
		class end;

		virtual ~Stream(){};

		/** Read & Write
		 * NOTE: They throws exception
		 */
		virtual unsigned Read(void*, unsigned maxSize) = 0;
		virtual unsigned Write(const void*, unsigned size) = 0;

		/** << and >> operator, flush
		 * NOTE: specialize template if you want to use your type
		 */
		Stream& operator>>(String&);
		Stream& operator<<(const String&);
		Stream& operator<<(const char*);
		Stream& operator<<(const end&){ Flush(); return *this; };
		Stream& operator<<(const endl&);
		void Flush();

	protected:
		Stream(){};
	};
}
