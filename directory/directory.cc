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
#include <syslog.h>

#include <toolbox/directory/directory.h>


namespace TB{


	/** ディレクトリ内の各ファイル
	 */

	Directory::Node::Node(Directory& dir, const dirent& ent) :
		List<Node>::Node(true),
		name(ent.d_name),
		type(ent.d_type){
//		strcpy(name, ent.d_name);
		dir.entries.Add(*this);
	}

	bool Directory::Node::IsDir() const { return type ==  DT_DIR; }
	bool Directory::Node::IsRegular(bool isl) const {
		return type == DT_REG || (isl && IsLink());
	}
	bool Directory::Node::IsBlock() const { return type ==  DT_BLK; }
	bool Directory::Node::IsChar() const { return type ==  DT_CHR; }
	bool Directory::Node::IsFifo() const { return type ==  DT_FIFO; }
	bool Directory::Node::IsLink() const { return type ==  DT_LNK; }
	bool Directory::Node::IsSocket() const { return type ==  DT_SOCK; }


	/** ディレクトリの読み込み
	 */
	Directory::Directory(const char* path){
		DIR* d(opendir(path));
		if(!d){
			syslog(LOG_ERR,"could not open dir:%s", path);
		}
		while(dirent* ent = readdir(d)){
			if((*ent).d_type != DT_UNKNOWN){
				new Node(*this, *ent);
			}
		}
		closedir(d);
	}
	Directory::~Directory(){}

}
