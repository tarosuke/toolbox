#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "directory.h"


namespace wOToolbox{

	Directory::Directory(const char* path) :
		entries(0), numOfEntries(scandir(path, &entries, &DefaultFilter, 0)){}
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
	void Directory::ITOR::operator++(){
		if(dir.numOfEntries <= index){
			++index;
		}
	}
	const char* Directory::ITOR::Name(){
		if(IsEnd()){ return 0; };
		return (*dir.entries[index]).d_name;
	}
	bool Directory::ITOR::IsDir(){
		return !IsEnd() && !!((*dir.entries[index]).d_type & DT_DIR);
	}

}
