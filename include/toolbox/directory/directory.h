/**
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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


extern "C"{
	struct dirent;
}

namespace TB{

	class Directory{
		Directory(const Directory&);
		void operator=(const Directory&);
	public:
		class ITOR{
			ITOR();
			ITOR(const ITOR&);
			void operator=(const ITOR&);
		public:
			ITOR(Directory&);
			ITOR& operator++();
			bool IsEnd(){ return index < 0 ||  dir.numOfEntries <= index; };
			operator bool(){ return !IsEnd(); };
			const char* Name();
			bool IsDir();
			bool IsReg();
			bool IsFile();
		private:
			int index;
			Directory& dir;
		};

		using Comp=int(*)(const dirent**, const dirent**);
		static int ByNameUp(const dirent**, const dirent**);
		static int ByNameDown(const dirent**, const dirent**);
		static int ByMTimeUp(const dirent**, const dirent**);
		static int ByMTimeDown(const dirent**, const dirent**);

		Directory(const char* path="./", Comp=0);
		~Directory();

	private:
		dirent** entries;
		int numOfEntries;

		static int DefaultFilter(const dirent*);
		static long long GetMTime(const dirent**);
	};

}
