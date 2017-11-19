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


#include <toolbox/container/list.h>
#include <toolbox/string.h>


namespace TB{

	class Directory{
		Directory(const Directory&);
		void operator=(const Directory&);
	public:
		//各ファイルエントリ
		class Node : public List<Node>::Node{
			Node();
			Node(const Node&);
			void operator=(const Node&);
		public:
			Node(const dirent&);

			bool IsDir() const;
			bool IsRegular(bool includeSymLinks = true) const;
			bool IsBlock() const; //block device
			bool IsChar() const; // char device
			bool IsFifo() const; // named pipe
			bool IsLink() const; //symbolic link
			bool IsSocket() const; //UNIX domain socket

			operator const char*() const { return (const char*)name; };

		private:
			 String name; //ファイル名
			const unsigned char type; //direntのd_typeと同じ
		};

		//反復子
		class Itor : public List<Node>::I{
			Itor();
			Itor(const Itor&);
			void operator=(const Itor&);
		public:
			Itor(Directory& d) : List<Node>::I(d.entries){};
		};

		//フィルタ件ソータのインターフェイス
		class Filter{
			Filter(const Filter&);
			void operator=(const Filter&);
		public:
			/** Filter
			 * return true if the node is valid
			 */
			virtual bool IsValid(const dirent&) const { return true; };

			/** compare two nodes
			 * return true if first parametor is previous than second one.
			 */
			virtual bool IsPrevious(const Node&, const Node&) const { return true; };
		};

		//エントリの読み込み
		Directory(const char* path="./", Filter* =0);
		~Directory();

	private:
		List<Node> entries;
	};

}
