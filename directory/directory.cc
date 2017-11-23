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

	Directory::Node::Node(const dirent& ent) :
		List<Node>::Node(true),
		name(ent.d_name),
		type(ent.d_type),
		atime(0), mtime(0), ctime(0){
		struct stat fileStat;
		if(!stat(ent.d_name, &fileStat)){
			atime = fileStat.st_atim.tv_sec;
			mtime = fileStat.st_mtim.tv_sec;
			ctime = fileStat.st_ctim.tv_sec;
		}else{
			syslog(LOG_ERR,"could not stat file:%s", ent.d_name);
		}
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
	Directory::Directory(const char* path, Filter* f){
		DIR* d(opendir(path));
		if(!d){
			syslog(LOG_ERR,"could not open dir:%s", path);
		}
		while(dirent* ent = readdir(d)){
			if((*ent).d_type != DT_UNKNOWN && (!f || (*f).IsValid(*ent))){
				if(auto* n = new Node(*ent)){
					if(!f){
						//フィルタが設定されていないのでそのまま追加
						entries.Add(*n);
					}else{
						//フィルタが設定されているので挿入ソート
						for(List<Node>::I i(entries); ++i;){
							if((*f).IsPrevious(*n, *i)){
								//ここに挿入
								i.Insert(*n);
								goto Inserted;
							}
						}
						entries.Add(*n);
Inserted:;
					}
				}
			}
		}
		closedir(d);
	}
	Directory::~Directory(){}

}
