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

#include "../exception.h"



namespace TB{
	/** Stream
	 * Interface of Stream series
	 */
	class Stream{
		Stream(const Stream&);
		void operator=(const Stream&);
	public:
		virtual ~Stream(){};

		/** Read & Write
		 * NOTE: They throws exception
		 */
		virtual unsigned Read(void*, unsigned maxSize) = 0;
		virtual unsigned Write(const void*, unsigned size) = 0;

		template<typename T> unsigned Read(T& b){
			return Read(&b, sizeof(T)) / sizeof(T);
		};
		template<typename T> unsigned Write(const T& b){
			return Write(&b, sizeof(T)) / sizeof(T);
		};

	protected:
		Stream(){};
	};
}
