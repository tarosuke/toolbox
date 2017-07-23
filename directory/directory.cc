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

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <toolbox/directory/directory.h>


namespace TB{

	Directory::Directory(const char* path, Comp c) :
		entries(0), numOfEntries(scandir(path, &entries, &DefaultFilter, c)){}
	Directory::~Directory(){
		if(entries){
			free(entries);
		}
	}

	//ディレクトリそれ自体は除外するscandicのフィルタ
	int Directory::DefaultFilter(const dirent* e){
		return strcmp(".", (*e).d_name);
	}


	Directory::ITOR::ITOR(Directory& dir) : index(0), dir(dir){}
	Directory::ITOR& Directory::ITOR::operator++(){
		if(index < dir.numOfEntries){
			++index;
		}
		return *this;
	}
	const char* Directory::ITOR::Name(){
		if(IsEnd()){ return 0; };
		return (*dir.entries[index]).d_name;
	}
	bool Directory::ITOR::IsDir(){
		return !IsEnd() && !!((*dir.entries[index]).d_type & DT_DIR);
	}
	bool Directory::ITOR::IsReg(){
		return !IsEnd() && !!((*dir.entries[index]).d_type & DT_REG);
	}
	bool Directory::ITOR::IsFile(){
		return !IsEnd() && !!((*dir.entries[index]).d_type & (DT_REG | DT_LNK));
	}

	/** ソート
	 */
	int Directory::ByNameUp(const dirent** a, const dirent** b){
		return versionsort(a, b);
	}
	int Directory::ByNameDown(const dirent** a, const dirent** b){
		return -versionsort(a, b);
	}

	long long Directory::GetMTime(const dirent** d){
		struct stat s;
		if(stat((**d).d_name, &s) < 0){
			return -1;
		}
		return s.st_mtim.tv_sec * 1000000000LL + s.st_mtim.tv_nsec;

	}
	int Directory::ByMTimeUp(const dirent** a, const dirent** b){
		const long long aTime(GetMTime(a));
		const long long bTime(GetMTime(b));

		if(aTime < 0 || bTime < 0){
			//情報を取得できなかった
			return 0;
		}

		return bTime - aTime;
	}
	int Directory::ByMTimeDown(const dirent** a, const dirent** b){
		const long long aTime(GetMTime(a));
		const long long bTime(GetMTime(b));

		if(aTime < 0 || bTime < 0){
			//情報を取得できなかった
			return 0;
		}

		return aTime - bTime;
	}


}
