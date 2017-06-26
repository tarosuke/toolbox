#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <toolbox/directory/directory.h>


namespace TB{

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

}
